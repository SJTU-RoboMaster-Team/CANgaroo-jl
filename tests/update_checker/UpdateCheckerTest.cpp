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

#include <functional>

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtTest>

#include "core/UpdateChecker.h"

Q_DECLARE_METATYPE(UpdateCheckResult)

namespace {

class MockHttpServer : public QTcpServer
{
public:
    using Handler = std::function<void(const QByteArray &request, QTcpSocket *socket)>;

    explicit MockHttpServer(Handler handler, QObject *parent = nullptr)
        : QTcpServer(parent)
        , _handler(std::move(handler))
    {
        connect(this, &QTcpServer::newConnection, this, [this]() {
            while (hasPendingConnections())
            {
                QTcpSocket *sock = nextPendingConnection();
                auto *buffer = new QByteArray();
                connect(sock, &QTcpSocket::readyRead, this, [this, sock, buffer]() {
                    buffer->append(sock->readAll());
                    if (buffer->contains("\r\n\r\n"))
                    {
                        const QByteArray req = *buffer;
                        buffer->clear();
                        if (_handler)
                            _handler(req, sock);
                    }
                });
                connect(sock, &QTcpSocket::disconnected, this, [sock, buffer]() {
                    delete buffer;
                    sock->deleteLater();
                });
            }
        });
    }

    QString baseUrl() const
    {
        return QStringLiteral("http://127.0.0.1:%1").arg(serverPort());
    }

private:
    Handler _handler;
};

QByteArray buildHttpResponse(int statusCode, const QByteArray &statusText,
                             const QByteArray &body,
                             const QByteArray &extraHeaders = {})
{
    QByteArray resp;
    resp += "HTTP/1.1 " + QByteArray::number(statusCode) + " " + statusText + "\r\n";
    resp += "Content-Type: application/json; charset=utf-8\r\n";
    resp += "Content-Length: " + QByteArray::number(body.size()) + "\r\n";
    resp += "Connection: close\r\n";
    if (!extraHeaders.isEmpty())
        resp += extraHeaders;
    resp += "\r\n";
    resp += body;
    return resp;
}

} // namespace

class UpdateCheckerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultRepositoryIsFork();
    void parseAndCompareVersions();
    void evaluateReleaseJson_updateAvailableAndAssets();
    void evaluateReleaseJson_upToDateDevAndErrors();
    void evaluateTagsJson_picksHighestVersion();
    void httpLatestReleaseSuccess();
    void httpFallsBackToTagsOn404();
    void httpReportsRateLimit();
    void cancelAbortsInFlightCheck();
};

void UpdateCheckerTest::initTestCase()
{
    qRegisterMetaType<UpdateCheckResult>();
}

void UpdateCheckerTest::defaultRepositoryIsFork()
{
    UpdateChecker checker;
    QCOMPARE(checker.repository(), QStringLiteral("SJTU-RoboMaster-Team/CANgaroo-jl"));
    QCOMPARE(UpdateChecker::repositoryWebUrl(),
             QStringLiteral("https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl"));
    QCOMPARE(UpdateChecker::releasesPageUrl(),
             QStringLiteral("https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases"));
}

void UpdateCheckerTest::parseAndCompareVersions()
{
    QCOMPARE(UpdateChecker::compareVersionStrings("0.7.2", "V0.7.2"), std::optional<int>(0));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.7", "0.7.0"), std::optional<int>(0));
    QCOMPARE(UpdateChecker::compareVersionStrings("v0.7.2", "V0.7.3"), std::optional<int>(-1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.10.0", "0.7.2"), std::optional<int>(1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.7.2.1", "0.7.2"), std::optional<int>(1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.8.0-rc1", "0.8.0"), std::optional<int>(-1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.8.0-alpha.10", "0.8.0-alpha.2"), std::optional<int>(1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.8.0-beta.1", "0.8.0-alpha.9"), std::optional<int>(1));
    QCOMPARE(UpdateChecker::compareVersionStrings("0.8.0+build42", "0.8.0"), std::optional<int>(0));

    QVERIFY(!UpdateChecker::parseVersion("").has_value());
    QVERIFY(!UpdateChecker::parseVersion("dev").has_value());
    QVERIFY(!UpdateChecker::parseVersion("v").has_value());
    QVERIFY(!UpdateChecker::parseVersion("0..2").has_value());
    QVERIFY(!UpdateChecker::parseVersion("0.7.x").has_value());
    QVERIFY(!UpdateChecker::compareVersionStrings("dev", "0.7.2").has_value());
}

void UpdateCheckerTest::evaluateReleaseJson_updateAvailableAndAssets()
{
    const QByteArray payload = QJsonDocument(QJsonObject{
        { "tag_name", "V0.8.0" },
        { "name", "CANgaroo V0.8.0" },
        { "body", "- Added RoboMaster motor decoder\n- Fixed CAN-FD export" },
        { "published_at", "2026-12-01T10:00:00Z" },
        { "html_url", "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/tag/V0.8.0" },
        { "draft", false },
        { "assets", QJsonArray{
            QJsonObject{
                { "name", "CANgaroo-0.8.0-windows-x64.zip.sha256" },
                { "browser_download_url", "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/download/V0.8.0/CANgaroo-0.8.0-windows-x64.zip.sha256" }
            },
            QJsonObject{
                { "name", "CANgaroo-0.8.0-windows-x64.zip" },
                { "browser_download_url", "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/download/V0.8.0/CANgaroo-0.8.0-windows-x64.zip" }
            },
            QJsonObject{
                { "name", "CANgaroo-0.8.0-x86_64.AppImage" },
                { "browser_download_url", "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/download/V0.8.0/CANgaroo-0.8.0-x86_64.AppImage" }
            },
            QJsonObject{
                { "name", "cangaroo_0.8.0_amd64.deb" },
                { "browser_download_url", "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/download/V0.8.0/cangaroo_0.8.0_amd64.deb" }
            }
        } }
    }).toJson();

    const UpdateCheckResult winRes = UpdateChecker::evaluateLatestReleasePayload(
        payload, "0.7.2", QString::fromLatin1(UpdateChecker::defaultRepository()), "windows");
    QCOMPARE(winRes.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(winRes.sourceKind, UpdateSourceKind::Release);
    QCOMPARE(winRes.latestTag, QStringLiteral("V0.8.0"));
    QCOMPARE(winRes.latestVersion, QStringLiteral("0.8.0"));
    QCOMPARE(winRes.assetName, QStringLiteral("CANgaroo-0.8.0-windows-x64.zip"));
    QVERIFY(winRes.downloadUrl.endsWith(QStringLiteral("CANgaroo-0.8.0-windows-x64.zip")));
    QVERIFY(winRes.releaseNotes.contains(QStringLiteral("RoboMaster motor decoder")));

    const UpdateCheckResult appImageRes = UpdateChecker::evaluateLatestReleasePayload(
        payload, "0.7.2", QString::fromLatin1(UpdateChecker::defaultRepository()), "appimage");
    QCOMPARE(appImageRes.assetName, QStringLiteral("CANgaroo-0.8.0-x86_64.AppImage"));

    const UpdateCheckResult debRes = UpdateChecker::evaluateLatestReleasePayload(
        payload, "0.7.2", QString::fromLatin1(UpdateChecker::defaultRepository()), "linux");
    QCOMPARE(debRes.assetName, QStringLiteral("cangaroo_0.8.0_amd64.deb"));
}

void UpdateCheckerTest::evaluateReleaseJson_upToDateDevAndErrors()
{
    const QByteArray sameVer = R"({"tag_name":"V0.7.2","html_url":"https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/tag/V0.7.2"})";

    const UpdateCheckResult upToDate = UpdateChecker::evaluateLatestReleasePayload(sameVer, "0.7.2");
    QCOMPARE(upToDate.status, UpdateCheckResult::Status::UpToDate);
    QCOMPARE(upToDate.latestVersion, QStringLiteral("0.7.2"));

    const UpdateCheckResult newerLocal = UpdateChecker::evaluateLatestReleasePayload(sameVer, "0.8.0");
    QCOMPARE(newerLocal.status, UpdateCheckResult::Status::UpToDate);

    const UpdateCheckResult devBuild = UpdateChecker::evaluateLatestReleasePayload(sameVer, "dev");
    QCOMPARE(devBuild.status, UpdateCheckResult::Status::DevBuild);
    QCOMPARE(devBuild.latestTag, QStringLiteral("V0.7.2"));

    const UpdateCheckResult draftErr = UpdateChecker::evaluateLatestReleasePayload(R"({"tag_name":"V0.9.0","draft":true})", "0.7.2");
    QCOMPARE(draftErr.status, UpdateCheckResult::Status::Error);

    const UpdateCheckResult badJson = UpdateChecker::evaluateLatestReleasePayload("{not json", "0.7.2");
    QCOMPARE(badJson.status, UpdateCheckResult::Status::Error);
}

void UpdateCheckerTest::evaluateTagsJson_picksHighestVersion()
{
    const QByteArray tagsJson = QJsonDocument(QJsonArray{
        QJsonObject{ { "name", "nightly-test" } },
        QJsonObject{ { "name", "V0.7.1" } },
        QJsonObject{ { "name", "V0.7.3" } },
        QJsonObject{ { "name", "V0.7.2" } }
    }).toJson();

    const UpdateCheckResult res = UpdateChecker::evaluateTagsPayload(tagsJson, "0.7.2");
    QCOMPARE(res.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(res.sourceKind, UpdateSourceKind::TagFallback);
    QCOMPARE(res.latestTag, QStringLiteral("V0.7.3"));
    QCOMPARE(res.latestVersion, QStringLiteral("0.7.3"));
    QCOMPARE(res.releasePageUrl,
             QStringLiteral("https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/tag/V0.7.3"));

    const UpdateCheckResult emptyRes = UpdateChecker::evaluateTagsPayload("[]", "0.7.2");
    QCOMPARE(emptyRes.status, UpdateCheckResult::Status::Error);
}

void UpdateCheckerTest::httpLatestReleaseSuccess()
{
    QByteArray seenRequest;
    MockHttpServer server([&](const QByteArray &req, QTcpSocket *sock) {
        seenRequest = req;
        const QByteArray body = R"({
            "tag_name": "V0.8.1",
            "name": "CANgaroo V0.8.1",
            "body": "Bug fixes",
            "html_url": "https://github.com/SJTU-RoboMaster-Team/CANgaroo-jl/releases/tag/V0.8.1",
            "assets": []
        })";
        sock->write(buildHttpResponse(200, "OK", body));
        sock->disconnectFromHost();
    });
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    UpdateChecker checker;
    checker.setApiBaseUrl(server.baseUrl());
    QSignalSpy finishedSpy(&checker, &UpdateChecker::checkFinished);

    checker.checkForUpdates("0.7.2");
    QVERIFY(checker.isChecking());
    QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.count(), 1, 5000);
    QVERIFY(!checker.isChecking());

    QVERIFY(seenRequest.startsWith("GET /repos/SJTU-RoboMaster-Team/CANgaroo-jl/releases/latest "));
    QVERIFY(seenRequest.contains("User-Agent: CANgaroo-jl/0.7.2"));

    const auto res = qvariant_cast<UpdateCheckResult>(finishedSpy.at(0).at(0));
    QCOMPARE(res.status, UpdateCheckResult::Status::UpdateAvailable);
    QCOMPARE(res.sourceKind, UpdateSourceKind::Release);
    QCOMPARE(res.latestVersion, QStringLiteral("0.8.1"));
}

void UpdateCheckerTest::httpFallsBackToTagsOn404()
{
    QList<QByteArray> paths;
    MockHttpServer server([&](const QByteArray &req, QTcpSocket *sock) {
        const qsizetype lineEnd = req.indexOf("\r\n");
        paths.append(req.left(lineEnd));
        if (req.contains("/releases/latest"))
        {
            sock->write(buildHttpResponse(404, "Not Found", R"({"message":"Not Found"})"));
        }
        else if (req.contains("/tags"))
        {
            sock->write(buildHttpResponse(200, "OK", R"([{"name":"V0.7.2"},{"name":"V0.7.1"}])"));
        }
        else
        {
            sock->write(buildHttpResponse(400, "Bad Request", "{}"));
        }
        sock->disconnectFromHost();
    });
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    UpdateChecker checker;
    checker.setApiBaseUrl(server.baseUrl());
    QSignalSpy finishedSpy(&checker, &UpdateChecker::checkFinished);

    checker.checkForUpdates("0.7.2");
    QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.count(), 1, 5000);

    QCOMPARE(paths.size(), 2);
    QVERIFY(paths[0].startsWith("GET /repos/SJTU-RoboMaster-Team/CANgaroo-jl/releases/latest "));
    QVERIFY(paths[1].startsWith("GET /repos/SJTU-RoboMaster-Team/CANgaroo-jl/tags?per_page=20 "));

    const auto res = qvariant_cast<UpdateCheckResult>(finishedSpy.at(0).at(0));
    QCOMPARE(res.status, UpdateCheckResult::Status::UpToDate);
    QCOMPARE(res.sourceKind, UpdateSourceKind::TagFallback);
    QCOMPARE(res.latestTag, QStringLiteral("V0.7.2"));
}

void UpdateCheckerTest::httpReportsRateLimit()
{
    MockHttpServer server([&](const QByteArray &, QTcpSocket *sock) {
        sock->write(buildHttpResponse(
            403, "Forbidden",
            R"({"message":"API rate limit exceeded"})",
            "X-RateLimit-Remaining: 0\r\n"));
        sock->disconnectFromHost();
    });
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    UpdateChecker checker;
    checker.setApiBaseUrl(server.baseUrl());
    QSignalSpy finishedSpy(&checker, &UpdateChecker::checkFinished);

    checker.checkForUpdates("0.7.2");
    QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.count(), 1, 5000);

    const auto res = qvariant_cast<UpdateCheckResult>(finishedSpy.at(0).at(0));
    QCOMPARE(res.status, UpdateCheckResult::Status::Error);
    QVERIFY(res.errorMessage.contains(QStringLiteral("rate limit"), Qt::CaseInsensitive));
}

void UpdateCheckerTest::cancelAbortsInFlightCheck()
{
    MockHttpServer server([&](const QByteArray &, QTcpSocket *) {
        // Hold the socket open without replying so cancel() aborts an active transfer.
    });
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    UpdateChecker checker;
    checker.setApiBaseUrl(server.baseUrl());
    QSignalSpy finishedSpy(&checker, &UpdateChecker::checkFinished);
    QSignalSpy cancelledSpy(&checker, &UpdateChecker::checkCancelled);

    checker.checkForUpdates("0.7.2");
    QVERIFY(checker.isChecking());
    QTest::qWait(50);
    checker.cancel();

    QTRY_COMPARE_WITH_TIMEOUT(cancelledSpy.count(), 1, 5000);
    QVERIFY(!checker.isChecking());
    QCOMPARE(finishedSpy.count(), 0);
}

QTEST_MAIN(UpdateCheckerTest)
#include "UpdateCheckerTest.moc"
