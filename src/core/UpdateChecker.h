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

#pragma once

#include <optional>

#include <QByteArray>
#include <QJsonArray>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QVector>

class QNetworkAccessManager;
class QNetworkReply;

struct SemanticVersion
{
    QVector<int> parts;
    QString preRelease;

    [[nodiscard]] bool isValid() const { return !parts.isEmpty(); }
    [[nodiscard]] bool isPreRelease() const { return !preRelease.isEmpty(); }
};

enum class UpdateSourceKind
{
    Release,
    TagFallback,
    ReleasesAtom
};

struct UpdateCheckResult
{
    enum class Status
    {
        UpToDate,
        UpdateAvailable,
        DevBuild,
        Error
    };

    Status status = Status::Error;
    UpdateSourceKind sourceKind = UpdateSourceKind::Release;

    QString repository;
    QString currentVersion;
    QString latestVersion;
    QString latestTag;
    QString releaseName;
    QString releaseNotes;
    QString publishedAt;
    QString releasePageUrl;
    QString downloadUrl;
    QString assetName;
    QString errorMessage;
};

class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    static constexpr const char *defaultRepository() { return "SJTU-RoboMaster-Team/CANgaroo-jl"; }
    static constexpr const char *defaultWebBaseUrl() { return "https://github.com"; }
    static constexpr const char *defaultApiBaseUrl() { return "https://api.github.com"; }

    explicit UpdateChecker(QObject *parent = nullptr);
    ~UpdateChecker() override;

    void setRepository(const QString &repository);
    [[nodiscard]] QString repository() const;

    void setApiBaseUrl(const QString &apiBaseUrl);
    [[nodiscard]] QString apiBaseUrl() const;

    void setWebBaseUrl(const QString &webBaseUrl);
    [[nodiscard]] QString webBaseUrl() const;

    [[nodiscard]] bool isChecking() const;

    void checkForUpdates(const QString &currentVersion);
    void cancel();

    [[nodiscard]] static QString repositoryWebUrl(
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));
    [[nodiscard]] static QString releasesPageUrl(
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));

    /// Atom feed of the published releases, used when the GitHub API is rate limited.
    [[nodiscard]] static QString releasesAtomUrl(
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));

    [[nodiscard]] static QString normalizeVersionString(const QString &raw);
    [[nodiscard]] static std::optional<SemanticVersion> parseVersion(const QString &versionString);
    [[nodiscard]] static int compareVersions(const SemanticVersion &lhs, const SemanticVersion &rhs);
    [[nodiscard]] static std::optional<int> compareVersionStrings(const QString &lhs, const QString &rhs);

    [[nodiscard]] static QString currentPlatformKey();
    [[nodiscard]] static QString selectBestAssetUrl(
        const QJsonArray &assets,
        QString *outAssetName = nullptr,
        const QString &platformKey = currentPlatformKey());

    [[nodiscard]] static UpdateCheckResult evaluateLatestReleasePayload(
        const QByteArray &jsonBytes,
        const QString &currentVersion,
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &platformKey = currentPlatformKey(),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));

    [[nodiscard]] static UpdateCheckResult evaluateTagsPayload(
        const QByteArray &jsonBytes,
        const QString &currentVersion,
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));

    [[nodiscard]] static UpdateCheckResult evaluateReleasesAtomPayload(
        const QByteArray &atomBytes,
        const QString &currentVersion,
        const QString &repository = QString::fromLatin1(defaultRepository()),
        const QString &webBaseUrl = QString::fromLatin1(defaultWebBaseUrl()));

signals:
    void checkFinished(const UpdateCheckResult &result);
    void checkCancelled();

private:
    enum class RequestPhase
    {
        LatestRelease,
        TagsFallback,
        ReleasesAtom
    };

    void startRequest(const QUrl &url, RequestPhase phase);
    void startReleasesAtomFallback();
    void handleReplyFinished(QNetworkReply *reply, RequestPhase phase);
    void finishWithError(const QString &errorMessage);

    QNetworkAccessManager *_nam = nullptr;
    QPointer<QNetworkReply> _activeReply;

    QString _repository;
    QString _apiBaseUrl;
    QString _webBaseUrl;
    QString _currentVersion;

    bool _checking = false;
    bool _cancelled = false;
};
