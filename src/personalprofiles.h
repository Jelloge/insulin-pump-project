#ifndef PERSONALPROFILES_H
#define PERSONALPROFILES_H

#include "profile.h"
#include "timeProfile.h"
#include "timedsettingsprofiles.h"
#include "timedSettingsUtils.h"

#include <QWidget>
#include <QListWidget>
#include <QJsonObject>
#include <QObject>
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QFileInfo>
#include <QStandardPaths>

namespace Ui {
class personalProfiles;
}

class personalProfiles : public QWidget
{
    Q_OBJECT

public:
    explicit personalProfiles(QWidget *parent = nullptr);
    ~personalProfiles();

    void loadProfilesFromFile();
    void disableProfileButtons();
    void setProfileSetUpPage();

public slots:
    void handleNewTimeSegment(QJsonObject segment, int targetPageIndex = 3);

signals:
    void returnToMain();
    void returnToOptions();
    void timedSettingsUpdated();
    void switchToTimedSettingsPage();
    void addAndSwitchToTimedSettingsPage(QJsonObject segment);

private slots:
    void on_addProfileButton_clicked();
    void on_virtualKeyPressed();
    void on_checkButton_clicked();
    void on_basalRateButton_clicked();
    void on_timedSettingsButton_clicked();
    void on_correctionFactorButton_clicked();
    void on_carbRatioButton_clicked();
    void on_targetBGButton_clicked();
    void on_checkButton_Timed_clicked();
    void on_insulinDurationButton_clicked();
    void on_bolusSettingsButton_clicked();
    void on_maxBolusButton_clicked();
    void on_checkButton_Bolus_clicked();
    void on_tandemLogoButton_clicked();
    void on_carbohydratesCheckBox_stateChanged(int state);
    void on_checkButton_TimedBolus_clicked();

    void on_backButton_ProfileMain_clicked();
    void on_backButton_Keyboard_clicked();
    void on_backButton_ProfileSetUp_clicked();
    void on_backButton_TimedSettings_clicked();
    void on_backButton_BolusSettings_clicked();

    void on_profileListWidget_itemClicked(QListWidgetItem *item);
    void on_deleteProfileButton_clicked();
    void on_editProfileButton_clicked();
    void on_duplicateProfileButton_clicked();
    void on_activateProfileButton_clicked();

private:
    Ui::personalProfiles *ui;
    profile *newProfile;
    timeProfile *newTimeProfile;

    QString selectedProfileName;
    QString activeProfileName;
    QString getProfilesFilePath();
    QString editingProfileTime;

    QJsonArray loadProfiles();
    int findProfileIndex(const QString &name, const QJsonArray &profiles);
    bool saveProfiles(const QJsonArray &profiles);
};

#endif // PERSONALPROFILES_H
