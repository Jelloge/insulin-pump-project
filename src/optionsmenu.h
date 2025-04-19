#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "personalprofiles.h"
#include "timedsettingsprofiles.h"
#include "batterymanager.h"
#include "config.h"

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <QProgressDialog>

namespace Ui {
class optionsMenu;
}

class optionsMenu : public QWidget
{
    Q_OBJECT

public:
    explicit optionsMenu(QWidget *parent = nullptr);
    ~optionsMenu();

signals:
    void returnToMainWindow();
    void controlIQStatusUpdated(bool visible);
    void basalDeliveryStatusUpdated(bool visible);

private slots:
    void on_tandemLogoButton_clicked();
    void on_loadButton_clicked();
    void on_activityButton_clicked();
    void on_myCGMButton_clicked();
    void on_deviceSettingsButton_clicked();
    void on_historyButton_clicked();
    void on_stopInsulinButton_clicked();
    void on_myPumpButton_clicked();
    void on_personalProfilesButton_clicked();

    void on_backButton_OptionsMain_clicked();
    void on_xButton_clicked();
    void on_backButton_Load_clicked();
    void on_backButton_Activity_clicked();
    void on_backButton_MyPump_clicked();
    void on_backButton_MyCGM_clicked();
    void on_backButton_DeviceSettings_clicked();
    void on_backButton_History_clicked();

    void on_checkButton_clicked();
    void on_pumpInfoButton_clicked();
    void on_startStopSensorButton_clicked();
    void on_cgmInfoButton_clicked();
    void on_transmitterIDButton_clicked();
    void on_calibrateCGMButton_clicked();
    void on_cgmAlertsButton_clicked();
    void on_changeCartridgeButton_clicked();
    void on_fillTubingButton_clicked();
    void on_startFillInsulinButton_clicked();
    void on_fillCannulaButton_clicked();

    void on_siteReminderButton_clicked();
    void updateReminderButtonLabel();

    void on_lowBGToggleButton_clicked();

    void on_alertsRemindersButton_clicked();

    void on_lowBGButton_clicked();

    void on_highBGToggleButton_clicked();

    void on_highBGButton_clicked();

    void on_afterBolusBGToggleButton_clicked();

    void on_missedMealToggleButton_clicked();

    void on_afterBolusBGButton_clicked();

    void on_missedMealBolusButton_clicked();

    void on_backButton_AlertsReminders_clicked();

    void on_displaySettingsButton_clicked();

    void on_soundVolumeButton_clicked();

    void on_timeDateButton_clicked();

    void on_bluetoothSettingsButton_clicked();

    void on_controlIQToggleButton_clicked();

    void on_controlIQButton_clicked();

    void on_weightButton_clicked();

    void on_totalDailyInsulinButton_clicked();

    void on_backButton_ControlIQ_clicked();

    void on_checkButton_ControlIQ_clicked();

    void on_tempBasalRateToggleButton_clicked();

    void on_tempRateButton_clicked();

    void on_durationButton_clicked();

    void on_backButton_TempBasalRate_clicked();

    void on_checkButton_TempBasalRate_clicked();

    void on_tempBasalRatesButton_clicked();

private:
    Ui::optionsMenu *ui;
    personalProfiles *profilesPage = nullptr;
    timedSettingsProfiles *timeProfilesPage = nullptr;

    QTimer *fillTimer = nullptr;
    double amountFilled = 0.0;

    bool isFilling = false;
    bool lowBGAlertEnabled = false;
    bool highBGAlertEnabled = false;
    bool afterBolusBGAlertEnabled = false;
    bool missedMealAlertEnabled = false;
    bool bluetoothEnabled = false;
};

#endif // OPTIONSMENU_H
