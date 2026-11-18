/*
  Copyright (c) 2026 SJTU-RoboMaster-Team / CANgaroo contributors

  This file is part of cangaroo.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/UpdateChecker.h"

#include <algorithm>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace {

constexpr int kRequestTimeoutMs = 15000;

QString trimTrailingSlash(QString url)
{
    while (url.endsWith(QLatin1Char('/')))
        url.chop(1);
    return url;
}

bool isHttpUrl(const QUrl &url)
{
    if (!url.isValid())
        return false;
    const QString scheme = url.scheme().toLower();
    return scheme == QLatin1String("https") || scheme == QLatin1String("http");
}

int comparePreReleaseIdentifiers(const QString &lhs, const QString &rhs)
{
    if (lhs.isEmpty() && rhs.isEmpty())
        return 0;
    // Per SemVer 2.0.0: a normal version has higher precedence than a pre-release.
    if (lhs.isEmpty())
        return 1;
    if (rhs.isEmpty())
        return -1;

    const QStringList leftParts = lhs.split(QLatin1Char('.'), Qt::KeepEmptyParts);
    const QStringList rightParts = rhs.split(QLatin1Char('.'), Qt::KeepEmptyParts);
    const qsizetype count = std::min(leftParts.size(), rightParts.size());

    for (qsizetype i = 0; i < count; ++i)
    {
        const QString &a = leftParts.at(i);
        const QString &b = rightParts.at(i);
        if (a == b)
            continue;

        bool aOk = false;
        bool bOk = false;
        const int aNum = a.toInt(&aOk);
        const int bNum = b.toInt(&bOk);

        if (aOk && bOk)
            return aNum < bNum ? -1 : 1;
        // Numeric identifiers always have lower precedence than non-numeric identifiers.
        if (aOk != bOk)
            return aOk ? -1 : 1;

        const int cmp = QString::compare(a, b, Qt::CaseInsensitive);
        if (cmp != 0)
            return cmp < 0 ? -1 : 1;
    }

    if (leftParts.size() == rightParts.size())
        return 0;
    return leftParts.size() < rightParts.size() ? -1 : 1;
}

UpdateCheckResult buildComparisonResult(UpdateCheckResult base, const SemanticVersion &latestVer)
{
    const auto currentVer = UpdateChecker::parseVersion(base.currentVersion);
    if (!currentVer.has_value())
    {
        base.status = UpdateCheckResult::Status::DevBuild;
        return base;
    }

    const int cmp = UpdateChecker::compareVersions(latestVer, *currentVer);
    base.status = (cmp > 0) ? UpdateCheckResult::Status::UpdateAvailable
                            : UpdateCheckResult::Status::UpToDate;
    return base;
}

int scoreAssetForPlatform(const QString &nameLower, const QString &platformKey)
{
    if (nameLower.isEmpty())
        return -1;

    // Ignore checksum/signature metadata files when picking a primary installer/archive.
    if (nameLower.endsWith(QLatin1String(".sha256"))
        || nameLower.endsWith(QLatin1String(".sha512"))
        || nameLower.endsWith(QLatin1String(".sig"))
        || nameLower.endsWith(QLatin1String(".asc")))
    {
        return -1;
    }

    if (platformKey == QLatin1String("windows"))
    {
        if (nameLower.contains(QLatin1String("windows")) && nameLower.endsWith(QLatin1String(".zip")))
            return 100;
        if (nameLower.contains(QLatin1String("windows")) && nameLower.endsWith(QLatin1String(".exe")))
            return 95;
        if (nameLower.contains(QLatin1String("windows")) && nameLower.endsWith(QLatin1String(".msi")))
            return 90;
        if (nameLower.endsWith(QLatin1String(".exe")) || nameLower.endsWith(QLatin1String(".msi")))
            return 80;
        if (nameLower.contains(QLatin1String("win")) && nameLower.endsWith(QLatin1String(".zip")))
            return 75;
        return -1;
    }

    if (platformKey == QLatin1String("appimage"))
    {
        if (nameLower.endsWith(QLatin1String(".appimage")))
            return 100;
        if (nameLower.endsWith(QLatin1String(".deb")))
            return 70;
        if (nameLower.endsWith(QLatin1String(".tar.gz")) || nameLower.endsWith(QLatin1String(".tar.xz")))
            return 50;
        return -1;
    }

    if (platformKey == QLatin1String("linux"))
    {
        if (nameLower.endsWith(QLatin1String(".deb")))
            return 100;
        if (nameLower.endsWith(QLatin1String(".appimage")))
            return 90;
        if (nameLower.endsWith(QLatin1String(".rpm")))
            return 75;
        if (nameLower.endsWith(QLatin1String(".tar.gz")) || nameLower.endsWith(QLatin1String(".tar.xz")))
            return 50;
        return -1;
    }

    if (platformKey == QLatin1String("macos"))
    {
        if (nameLower.endsWith(QLatin1String(".dmg")))
            return 100;
        if ((nameLower.contains(QLatin1String("mac")) || nameLower.contains(QLatin1String("darwin"))
             || nameLower.contains(QLatin1String("osx")))
            && nameLower.endsWith(QLatin1String(".zip")))
        {
            return 85;
        }
        return -1;
    }

    return -1;
}

} // namespace

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent)
    , _nam(new QNetworkAccessManager(this))
    , _repository(QString::fromLatin1(defaultRepository()))
    , _apiBaseUrl(QString::fromLatin1(defaultApiBaseUrl()))
    , _webBaseUrl(QString::fromLatin1(defaultWebBaseUrl()))
{
}

UpdateChecker::~UpdateChecker()
{
    _checking = false;
    _cancelled = true;
    if (_activeReply)
    {
        disconnect(_activeReply, nullptr, this, nullptr);
        _activeReply->abort();
        _activeReply->deleteLater();
        _activeReply.clear();
    }
}

void UpdateChecker::setRepository(const QString &repository)
{
    const QString cleaned = trimTrailingSlash(repository.trimmed());
    if (!cleaned.isEmpty())
        _repository = cleaned;
}

QString UpdateChecker::repository() const
{
    return _repository;
}

void UpdateChecker::setApiBaseUrl(const QString &apiBaseUrl)
{
    const QString cleaned = trimTrailingSlash(apiBaseUrl.trimmed());
    if (!cleaned.isEmpty())
        _apiBaseUrl = cleaned;
}

QString UpdateChecker::apiBaseUrl() const
{
    return _apiBaseUrl;
}

void UpdateChecker::setWebBaseUrl(const QString &webBaseUrl)
{
    const QString cleaned = trimTrailingSlash(webBaseUrl.trimmed());
    if (!cleaned.isEmpty())
        _webBaseUrl = cleaned;
}

QString UpdateChecker::webBaseUrl() const
{
    return _webBaseUrl;
}

bool UpdateChecker::isChecking() const
{
    return _checking;
}

QString UpdateChecker::repositoryWebUrl(const QString &repository, const QString &webBaseUrl)
{
    return QStringLiteral("%1/%2").arg(trimTrailingSlash(webBaseUrl), trimTrailingSlash(repository));
}

QString UpdateChecker::releasesPageUrl(const QString &repository, const QString &webBaseUrl)
{
    return repositoryWebUrl(repository, webBaseUrl) + QStringLiteral("/releases");
}

QString UpdateChecker::normalizeVersionString(const QString &raw)
{
    QString s = raw.trimmed();
    if (s.size() >= 2
        && (s.at(0) == QLatin1Char('v') || s.at(0) == QLatin1Char('V'))
        && s.at(1).isDigit())
    {
        s.remove(0, 1);
    }
    return s;
}

std::optional<SemanticVersion> UpdateChecker::parseVersion(const QString &versionString)
{
    QString s = normalizeVersionString(versionString);
    if (s.isEmpty())
        return std::nullopt;

    // Strip build metadata (+...) as it does not participate in precedence ordering.
    const qsizetype plusIdx = s.indexOf(QLatin1Char('+'));
    if (plusIdx >= 0)
        s.truncate(plusIdx);

    QString core = s;
    QString preRelease;
    const qsizetype dashIdx = s.indexOf(QLatin1Char('-'));
    if (dashIdx >= 0)
    {
        core = s.left(dashIdx);
        preRelease = s.mid(dashIdx + 1).trimmed();
        if (preRelease.isEmpty())
            return std::nullopt;
    }

    const QStringList coreTokens = core.split(QLatin1Char('.'), Qt::KeepEmptyParts);
    if (coreTokens.isEmpty())
        return std::nullopt;

    SemanticVersion ver;
    ver.parts.reserve(std::max<qsizetype>(3, coreTokens.size()));

    for (const QString &token : coreTokens)
    {
        if (token.isEmpty())
            return std::nullopt;
        bool ok = false;
        const int num = token.toInt(&ok);
        if (!ok || num < 0)
            return std::nullopt;
        ver.parts.append(num);
    }

    while (ver.parts.size() < 3)
        ver.parts.append(0);

    ver.preRelease = preRelease;
    return ver;
}

int UpdateChecker::compareVersions(const SemanticVersion &lhs, const SemanticVersion &rhs)
{
    const qsizetype maxParts = std::max(lhs.parts.size(), rhs.parts.size());
    for (qsizetype i = 0; i < maxParts; ++i)
    {
        const int l = i < lhs.parts.size() ? lhs.parts.at(i) : 0;
        const int r = i < rhs.parts.size() ? rhs.parts.at(i) : 0;
        if (l != r)
            return l < r ? -1 : 1;
    }
    return comparePreReleaseIdentifiers(lhs.preRelease, rhs.preRelease);
}

std::optional<int> UpdateChecker::compareVersionStrings(const QString &lhs, const QString &rhs)
{
    const auto lVer = parseVersion(lhs);
    const auto rVer = parseVersion(rhs);
    if (!lVer.has_value() || !rVer.has_value())
        return std::nullopt;
    return compareVersions(*lVer, *rVer);
}

QString UpdateChecker::currentPlatformKey()
{
#if defined(Q_OS_WIN) || defined(_WIN32)
    return QStringLiteral("windows");
#elif defined(Q_OS_MACOS)
    return QStringLiteral("macos");
#else
    if (qEnvironmentVariableIsSet("APPIMAGE"))
        return QStringLiteral("appimage");
    return QStringLiteral("linux");
#endif
}

QString UpdateChecker::selectBestAssetUrl(
    const QJsonArray &assets,
    QString *outAssetName,
    const QString &platformKey)
{
    int bestScore = -1;
    QString bestUrl;
    QString bestName;

    for (const QJsonValue &val : assets)
    {
        if (!val.isObject())
            continue;
        const QJsonObject obj = val.toObject();
        const QString name = obj.value(QLatin1String("name")).toString();
        const QString urlStr = obj.value(QLatin1String("browser_download_url")).toString();
        const QUrl url(urlStr);
        if (!isHttpUrl(url))
            continue;

        const int score = scoreAssetForPlatform(name.toLower(), platformKey);
        if (score > bestScore)
        {
            bestScore = score;
            bestUrl = url.toString();
            bestName = name;
        }
    }

    if (outAssetName)
        *outAssetName = bestName;
    return bestUrl;
}

UpdateCheckResult UpdateChecker::evaluateLatestReleasePayload(
    const QByteArray &jsonBytes,
    const QString &currentVersion,
    const QString &repository,
    const QString &platformKey,
    const QString &webBaseUrl)
{
    UpdateCheckResult result;
    result.repository = repository;
    result.currentVersion = currentVersion;
    result.sourceKind = UpdateSourceKind::Release;
    result.releasePageUrl = releasesPageUrl(repository, webBaseUrl);

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonBytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject())
    {
        result.status = UpdateCheckResult::Status::Error;
        result.errorMessage = tr("Invalid release metadata received from GitHub (%1).")
                                  .arg(parseError.errorString());
        return result;
    }

    const QJsonObject obj = doc.object();
    if (obj.value(QLatin1String("draft")).toBool(false))
    {
        result.status = UpdateCheckResult::Status::Error;
        result.errorMessage = tr("Latest release entry in %1 is still marked as draft.")
                                  .arg(repository);
        return result;
    }

    QString tagName = obj.value(QLatin1String("tag_name")).toString().trimmed();
    const QString releaseName = obj.value(QLatin1String("name")).toString().trimmed();
    if (tagName.isEmpty())
        tagName = releaseName;

    const auto parsedLatest = parseVersion(tagName);
    if (!parsedLatest.has_value())
    {
        result.status = UpdateCheckResult::Status::Error;
        result.errorMessage = tr("Could not parse release version \"%1\" from %2.")
                                  .arg(tagName, repository);
        return result;
    }

    result.latestTag = tagName;
    result.latestVersion = normalizeVersionString(tagName);
    result.releaseName = releaseName;
    result.releaseNotes = obj.value(QLatin1String("body")).toString().trimmed();
    result.publishedAt = obj.value(QLatin1String("published_at")).toString().trimmed();

    const QUrl htmlUrl(obj.value(QLatin1String("html_url")).toString().trimmed());
    if (isHttpUrl(htmlUrl))
        result.releasePageUrl = htmlUrl.toString();
    else
        result.releasePageUrl = releasesPageUrl(repository, webBaseUrl) + QLatin1String("/tag/") + tagName;

    result.downloadUrl = selectBestAssetUrl(
        obj.value(QLatin1String("assets")).toArray(),
        &result.assetName,
        platformKey);

    return buildComparisonResult(std::move(result), *parsedLatest);
}

UpdateCheckResult UpdateChecker::evaluateTagsPayload(
    const QByteArray &jsonBytes,
    const QString &currentVersion,
    const QString &repository,
    const QString &webBaseUrl)
{
    UpdateCheckResult result;
    result.repository = repository;
    result.currentVersion = currentVersion;
    result.sourceKind = UpdateSourceKind::TagFallback;
    result.releasePageUrl = releasesPageUrl(repository, webBaseUrl);

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonBytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray())
    {
        result.status = UpdateCheckResult::Status::Error;
        result.errorMessage = tr("Invalid tag metadata received from GitHub (%1).")
                                  .arg(parseError.errorString());
        return result;
    }

    std::optional<SemanticVersion> bestVer;
    QString bestTag;

    const QJsonArray tags = doc.array();
    for (const QJsonValue &val : tags)
    {
        if (!val.isObject())
            continue;
        const QString tagName = val.toObject().value(QLatin1String("name")).toString().trimmed();
        const auto parsed = parseVersion(tagName);
        if (!parsed.has_value())
            continue;

        if (!bestVer.has_value() || compareVersions(*parsed, *bestVer) > 0)
        {
            bestVer = parsed;
            bestTag = tagName;
        }
    }

    if (!bestVer.has_value())
    {
        result.status = UpdateCheckResult::Status::Error;
        result.errorMessage = tr("No published releases or version tags were found in %1.")
                                  .arg(repository);
        return result;
    }

    result.latestTag = bestTag;
    result.latestVersion = normalizeVersionString(bestTag);
    result.releasePageUrl = releasesPageUrl(repository, webBaseUrl) + QLatin1String("/tag/") + bestTag;

    return buildComparisonResult(std::move(result), *bestVer);
}

void UpdateChecker::checkForUpdates(const QString &currentVersion)
{
    if (_checking)
        return;

    _currentVersion = currentVersion.trimmed();
    _checking = true;
    _cancelled = false;

    const QUrl baseUrl(_apiBaseUrl);
    if (baseUrl.host() == QLatin1String("127.0.0.1") || baseUrl.host() == QLatin1String("localhost"))
        _nam->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
    else
        _nam->setProxy(QNetworkProxy(QNetworkProxy::DefaultProxy));

    const QUrl latestReleaseUrl(QStringLiteral("%1/repos/%2/releases/latest")
                                    .arg(_apiBaseUrl, _repository));
    startRequest(latestReleaseUrl, RequestPhase::LatestRelease);
}

void UpdateChecker::cancel()
{
    if (!_checking)
        return;

    _cancelled = true;
    if (_activeReply)
        _activeReply->abort();
}

void UpdateChecker::startRequest(const QUrl &url, RequestPhase phase)
{
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "application/vnd.github+json");
    req.setRawHeader("X-GitHub-Api-Version", "2022-11-28");
    const QByteArray userAgent = QStringLiteral("CANgaroo-jl/%1")
                                     .arg(_currentVersion.isEmpty() ? QStringLiteral("dev") : _currentVersion)
                                     .toUtf8();
    req.setRawHeader("User-Agent", userAgent);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setTransferTimeout(kRequestTimeoutMs);

    QNetworkReply *reply = _nam->get(req);
    _activeReply = reply;

    connect(reply, &QNetworkReply::finished, this, [this, reply, phase]() {
        handleReplyFinished(reply, phase);
    });
}

void UpdateChecker::handleReplyFinished(QNetworkReply *reply, RequestPhase phase)
{
    if (_activeReply == reply)
        _activeReply.clear();

    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QByteArray rateRemaining = reply->rawHeader("X-RateLimit-Remaining").trimmed();
    const QNetworkReply::NetworkError netErr = reply->error();
    const QString errStr = reply->errorString();

    if (_cancelled || netErr == QNetworkReply::OperationCanceledError)
    {
        reply->deleteLater();
        _checking = false;
        _cancelled = false;
        emit checkCancelled();
        return;
    }

    const QByteArray payload = (netErr == QNetworkReply::NoError && reply->isOpen())
        ? reply->readAll()
        : QByteArray();
    reply->deleteLater();

    // New forks / tag-only repos return 404 on /releases/latest; fall back to version tags.
    if (phase == RequestPhase::LatestRelease
        && (statusCode == 404 || netErr == QNetworkReply::ContentNotFoundError))
    {
        const QUrl tagsUrl(QStringLiteral("%1/repos/%2/tags?per_page=20")
                               .arg(_apiBaseUrl, _repository));
        startRequest(tagsUrl, RequestPhase::TagsFallback);
        return;
    }

    if ((statusCode == 403 || statusCode == 429) && rateRemaining == QByteArrayLiteral("0"))
    {
        finishWithError(tr("GitHub API rate limit exceeded. Please try again later or check the releases page directly."));
        return;
    }

    if (netErr != QNetworkReply::NoError)
    {
        if (statusCode > 0)
            finishWithError(tr("HTTP %1: %2").arg(statusCode).arg(errStr));
        else
            finishWithError(errStr);
        return;
    }

    UpdateCheckResult result = (phase == RequestPhase::LatestRelease)
        ? evaluateLatestReleasePayload(payload, _currentVersion, _repository, currentPlatformKey(), _webBaseUrl)
        : evaluateTagsPayload(payload, _currentVersion, _repository, _webBaseUrl);

    _checking = false;
    emit checkFinished(result);
}

void UpdateChecker::finishWithError(const QString &errorMessage)
{
    UpdateCheckResult result;
    result.status = UpdateCheckResult::Status::Error;
    result.repository = _repository;
    result.currentVersion = _currentVersion;
    result.releasePageUrl = releasesPageUrl(_repository, _webBaseUrl);
    result.errorMessage = errorMessage;

    _checking = false;
    emit checkFinished(result);
}
