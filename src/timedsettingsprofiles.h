#ifndef TIMEDSETTINGSPROFILES_H
#define TIMEDSETTINGSPROFILES_H

#include "timeProfile.h"
#include "timedSettingsUtils.h"

#include <QWidget>
#include <QJsonObject>
#include <QListWidget>
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
class timedSettingsProfiles;
}

class timedSettingsProfiles : public QWidget
{
    Q_OBJECT

public:
    explicit timedSettingsProfiles(QWidget *parent = nullptr);
    ~timedSettingsProfiles();

    void refreshTimedSettingsList();

public slots:
    void handleNewSegmentFromProfile(QJsonObject segment);

signals:
    void returnToMainPage();
    void returnToProfilesPage();
    void returnToTimedSettingsPage(QJsonObject timeSegment);

private slots:  
    void on_tandemLogoButton_clicked();
    void on_addTSButton_clicked();
    void on_startTimeButton_clicked();

    void on_backButton_TSMenu_clicked();
    void on_backButton_TimeSegment_clicked();

    void on_timeOfDayButton_clicked();
    void on_checkButton_TimeSegment_clicked();
    void on_selectTSButton_clicked();
    void on_deleteTSButton_clicked();
    void on_tSListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::timedSettingsProfiles *ui;
    timeProfile *newTimeProfile;
    QListWidgetItem *selectedTimeSegment = nullptr;

    QString getTimedSettingsFilePath();
    int findTimedSettingIndex(const QString &name, const QJsonArray &array);

};

#endif // TIMEDSETTINGSPROFILES_H
