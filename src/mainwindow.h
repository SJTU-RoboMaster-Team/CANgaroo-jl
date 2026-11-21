/*

  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
  Copyright (c) 2026 Schildkroet

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

#include <QMainWindow>
#include <QColor>
#include <QIcon>
#include <QList>
#include <QPointer>
#include "core/Backend.h"
#include <QSettings>

// VERSION_STRING is injected by qmake (DEFINES += VERSION_STRING=...).
// The fallback keeps IDEs happy when qmake definitions are not available.
#ifndef VERSION_STRING
#define VERSION_STRING "dev"
#endif

// Workspace file format version. Increment when breaking changes are made.
static constexpr int WORKSPACE_VERSION = 1;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QWidget;
class QDomElement;
class QActionGroup;
class QProgressDialog;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class ConfigurableWidget;
class SetupDialog;
class HardwareDialog;
class UpdateChecker;
struct UpdateCheckResult;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isMaximizedWindow();
    bool isDarkMode();

    void loadWorkspaceFromFile(const QString &filename);

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

public slots:
    QMainWindow *createTraceWindow(const QString &title = QString());
    QMainWindow *createGraphWindow(const QString &title = QString());
    void createStandaloneGraphWindow();
    QDockWidget *addGraphWidget(QMainWindow *parent = nullptr);
    QDockWidget *addRawTxWidget(QMainWindow *parent = nullptr);
    QDockWidget *addLogWidget(QMainWindow *parent = nullptr);
    QDockWidget *addStatusWidget(QMainWindow *parent = nullptr);
    QDockWidget *addTxGeneratorWidget(QMainWindow *parent = nullptr);
    QDockWidget *addReplayWidget(QMainWindow *parent = nullptr);
    QDockWidget *addMotorWidget(QMainWindow *parent = nullptr);

    bool showSetupDialog();
    void showAboutDialog();

    void startMeasurement();
    void stopMeasurement();
    void saveTraceToFile();

    void updateMeasurementActions();

private slots:
    void on_action_WorkspaceNew_triggered();
    void on_action_WorkspaceOpen_triggered();
    void on_action_WorkspaceSave_triggered();
    void on_action_WorkspaceSaveAs_triggered();
    void on_action_TraceClear_triggered();
    void on_actionCan_Status_View_triggered();
    void showSettingsDialog();

    void switchLanguage(QAction *action);
    void exportFullTrace();
    void importFullTrace();
    void reloadInterfaces();
    void showHardwareDialog();
    void checkForUpdates();
    void handleUpdateCheckFinished(const UpdateCheckResult &result);

private:
    Ui::MainWindow *ui;
    SetupDialog *_setupDlg = nullptr;
    QPointer<HardwareDialog> _hardwareDlg;
    QSettings settings;

    bool _workspaceModified = false;
    QString _workspaceFileName;
    QString _baseWindowTitle;
    bool _hasConfirmedSetup = false;
    bool _setupFromWorkspace = false; // setup was read from a workspace file, keep it as stored

    Backend &backend();

    QMainWindow *createTab(const QString &title, const char *titleSource = nullptr);
    QMainWindow *currentTab();
    // Closes the tab at `index` (no-op when it would close the last remaining
    // tab, so the window always keeps a workspace view). Marks the workspace
    // as modified and refreshes the tab close-button visibility.
    void closeTab(int index);

    // Constructor init helpers
    void initVersion();
    void initActions();
    void initDrivers();
    void initGeometry();
    void initWorkspace();
    void initAppearance();
    void initRecording();
    void initTriggerCapture();
    void initDisconnectBanner();

    // Continuous trace recording
    void showRecordingDialog();
    void updateRecordingStatus();

    // Trigger capture (blackbox)
    void showTriggerCaptureDialog();
    void updateTriggerStatus();

    // USB disconnect alert & manual rebind
    void onInterfaceConnectionLost(const QString &interfaceName, const QString &reason);
    void onManualRebindClicked();
    void applyDisconnectBannerTheme();
    void updateDisconnectBannerText();

    // Retranslation for widgets/menus/etc. that are built in code instead of a .ui file.
    void retranslateCustomUi();
    void retranslateTabAndDockTitles();

    // Theming helpers
    void applyCurrentTheme();
    void applyActionIcons();
    QIcon symbolicIcon(const QString &name) const;
    void applyTabBackgrounds();
    QColor tabBackgroundColor() const;

    // Dock factory helper: creates dock, sets widget, adds to parent, wires float/reparent.
    // Returns nullptr if parent is null (no active tab).
    QDockWidget *makeDock(const QString &title, const char *titleSource,
                          const QString &objectName, QWidget *content, QMainWindow *parent);

    void stopAndClearMeasurement();

    void clearWorkspace();
    bool loadWorkspaceTab(QDomElement el);
    bool loadWorkspaceSetup(QDomElement el);
    bool saveWorkspaceToFile(const QString &filename);

    void newWorkspace();
    void loadWorkspace();
    bool saveWorkspace();
    bool saveWorkspaceAs();

    void setWorkspaceModified(bool modified);
    int askSaveBecauseWorkspaceModified();

    QList<class GraphWindow*> _standaloneGraphWindows;

    // Toolbar button whose icon has to be recolored on every theme change.
    class QPushButton *_btnOpenGraph = nullptr;

    QAction *_actionStandaloneGraph = nullptr;
    QAction *_actionMotorView = nullptr;
    QAction *_actionCloseTab = nullptr;
    QAction *_actionExportFull = nullptr;
    QAction *_actionImportFull = nullptr;

    QAction *_actionRecord = nullptr;
    QAction *_actionRecordOptions = nullptr;
    QAction *_actionOpenFolder = nullptr;
    class QToolButton *_btnRecord = nullptr;
    class QLabel *_recordStatusLabel = nullptr;
    class QTimer *_recordStatusTimer = nullptr;

    QAction *_actionTriggerCapture = nullptr;
    class QToolButton *_btnTriggerCapture = nullptr;
    class QLabel *_triggerStatusLabel = nullptr;

    QWidget *_disconnectBanner = nullptr;
    class QLabel *_disconnectBannerLabel = nullptr;
    class QPushButton *_btnBannerRebind = nullptr;
    class QPushButton *_btnBannerDismiss = nullptr;
    QString _disconnectBannerInterface;
    QString _disconnectBannerReason;

    // Recent files
    static constexpr int MaxRecentFiles = 8;
    QMenu *m_recentFilesMenu = nullptr;
    void addToRecentFiles(const QString &filename);
    void updateRecentFilesMenu();

    void createLanguageMenu();
    void setupDockFloatReparent(QDockWidget *dock, QMainWindow *innerParent);
    void applyFontSize(int pointSize);
    QActionGroup *m_languageActionGroup = nullptr;

    void showUpdateAvailableDialog(const UpdateCheckResult &result);
    void closeUpdateProgressDialog();

    UpdateChecker *_updateChecker = nullptr;
    QPointer<QProgressDialog> _updateProgressDlg;
};
