#include "optionsmenu.h"
#include "ui_optionsmenu.h"

optionsMenu::optionsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::optionsMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Back Buttons
    QPixmap backPixmap(":/ui_icons/back.png");
    QIcon backIcon(backPixmap);

    ui->backButton_OptionsMain->setText("");
    ui->backButton_OptionsMain->setIcon(backIcon);
    ui->backButton_OptionsMain->setIconSize(QSize(55, 55));

    ui->backButton_Load->setText("");
    ui->backButton_Load->setIcon(backIcon);
    ui->backButton_Load->setIconSize(QSize(55, 55));

    ui->backButton_Activity->setText("");
    ui->backButton_Activity->setIcon(backIcon);
    ui->backButton_Activity->setIconSize(QSize(55, 55));

    ui->backButton_MyPump->setText("");
    ui->backButton_MyPump->setIcon(backIcon);
    ui->backButton_MyPump->setIconSize(QSize(55, 55));

    ui->backButton_MyCGM->setText("");
    ui->backButton_MyCGM->setIcon(backIcon);
    ui->backButton_MyCGM->setIconSize(QSize(55, 55));

    ui->backButton_DeviceSettings->setText("");
    ui->backButton_DeviceSettings->setIcon(backIcon);
    ui->backButton_DeviceSettings->setIconSize(QSize(55, 55));

    ui->backButton_History->setText("");
    ui->backButton_History->setIcon(backIcon);
    ui->backButton_History->setIconSize(QSize(55, 55));

    ui->backButton_AlertsReminders->setText("");
    ui->backButton_AlertsReminders->setIcon(backIcon);
    ui->backButton_AlertsReminders->setIconSize(QSize(55, 55));

    ui->backButton_ControlIQ->setText("");
    ui->backButton_ControlIQ->setIcon(backIcon);
    ui->backButton_ControlIQ->setIconSize(QSize(55, 55));

    ui->backButton_TempBasalRate->setText("");
    ui->backButton_TempBasalRate->setIcon(backIcon);
    ui->backButton_TempBasalRate->setIconSize(QSize(55, 55));

    // Close Button
    QPixmap xPixmap(":/ui_icons/close.png");
    QIcon xIcon(xPixmap);

    ui->xButton->setText("");
    ui->xButton->setIcon(xIcon);
    ui->xButton->setIconSize(QSize(25, 25));

    // Check Button
    QPixmap checkPixmap(":/ui_icons/check.png");
    QIcon checkIcon(checkPixmap);

    ui->checkButton->setText("");
    ui->checkButton->setIcon(checkIcon);
    ui->checkButton->setIconSize(QSize(30, 30));

    ui->checkButton_ControlIQ->setText("");
    ui->checkButton_ControlIQ->setIcon(checkIcon);
    ui->checkButton_ControlIQ->setIconSize(QSize(25, 25));

    ui->checkButton_TempBasalRate->setText("");
    ui->checkButton_TempBasalRate->setIcon(checkIcon);
    ui->checkButton_TempBasalRate->setIconSize(QSize(25, 25));

    profilesPage = new personalProfiles(this);
    timeProfilesPage = new timedSettingsProfiles(this);
    profilesPage->hide();
    timeProfilesPage->hide();

    // Connecting personalProfiles' signals
    connect(profilesPage, &personalProfiles::returnToMain, this, &optionsMenu::on_tandemLogoButton_clicked);
    connect(profilesPage, &personalProfiles::returnToOptions, this, &optionsMenu::show);
    connect(profilesPage, &personalProfiles::timedSettingsUpdated, timeProfilesPage, &timedSettingsProfiles::refreshTimedSettingsList);
    connect(profilesPage, &personalProfiles::switchToTimedSettingsPage, timeProfilesPage, &timedSettingsProfiles::show);
    connect(profilesPage, &personalProfiles::addAndSwitchToTimedSettingsPage,
            timeProfilesPage, &timedSettingsProfiles::handleNewSegmentFromProfile);

    // Connecting timeSettingsProfiles' signals
    connect(timeProfilesPage, &timedSettingsProfiles::returnToMainPage, this, &optionsMenu::on_tandemLogoButton_clicked);
    connect(timeProfilesPage, &timedSettingsProfiles::returnToProfilesPage, this, [=]() {
        profilesPage->show();
        profilesPage->setProfileSetUpPage();
    });
    connect(timeProfilesPage, &timedSettingsProfiles::returnToTimedSettingsPage, profilesPage,
        [=](QJsonObject segment) {
            profilesPage->handleNewTimeSegment(segment, 2);
        });

    // Set the battery label
    BatteryManager::instance()->updateLabel(ui->batteryLabel);

    // Clock update hookup
    connect(Config::instance(), &Config::clockUpdated, this, [=](const QString &time){
        ui->timeDateLabel->setText(time);
    });

    // Power / Battery buttons
    connect(ui->ChargeButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
    connect(ui->turnOffButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOff);
    connect(ui->turnOnButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOn);
}

optionsMenu::~optionsMenu()
{
    delete ui;
}

void optionsMenu::on_tandemLogoButton_clicked()
{
    emit returnToMainWindow();
    this->close();
}

// Stop Insulin Menu
void optionsMenu::on_stopInsulinButton_clicked()
{
    if (ui->stopInsulinButton->text() != "STOP INSULIN") {
        QMessageBox::information(this, "", "Resuming Insulin");
        ui->stopInsulinButton->setText("STOP INSULIN");
        ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
    } else {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void optionsMenu::on_checkButton_clicked()
{
    QString alarmTimer = ui->resumeInsulinComboBox->currentText();
    QMessageBox::information(this, "All Deliveries Stopped", "Resume Insulin Pump Alarm will sound after " + alarmTimer);
    ui->stopInsulinButton->setText("RESUME INSULIN");
    ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_xButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Load Menu
void optionsMenu::on_loadButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->tandemLogoButton->setEnabled(false);
    updateReminderButtonLabel();
}

// Change Cartridge
void optionsMenu::on_changeCartridgeButton_clicked()
{
    QMessageBox::StandardButton stopReply = QMessageBox::question(
        this,
        "Changing Cartridge",
        "In order to change a cartridge, all insulin deliveries will be stopped.\n\nDo you want to proceed?",
        QMessageBox::Yes | QMessageBox::No
    );
    if (stopReply != QMessageBox::Yes) return;

    QMessageBox::information(this, "Delivery Stopped", "All insulin deliveries have been stopped.");

    QMessageBox::StandardButton disconnectReply = QMessageBox::question(
        this,
        "Disconnect Infusion Set",
        "Make sure the infusion set is disconnected from your body.\n\nAre you ready to continue?",
        QMessageBox::Yes | QMessageBox::No
    );
    if (disconnectReply != QMessageBox::Yes) {
        QMessageBox::information(this, "Resumed", "Insulin delivery has resumed.");
        ui->stopInsulinButton->setText("STOP INSULIN");
        ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
        return;
    }

    ui->stopInsulinButton->setText("RESUME INSULIN");
    ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");

    QMessageBox *prepBox = new QMessageBox(this);
    prepBox->setWindowTitle("Preparing");
    prepBox->setText("Preparing for cartridge...");
    prepBox->setStandardButtons(QMessageBox::NoButton);
    prepBox->show();

    QTimer::singleShot(5000, this, [this, prepBox]() {
        prepBox->accept();

        auto showInstallPrompt = [this]() {
            while (true) {
                QMessageBox::StandardButton cartReply = QMessageBox::question(
                    this,
                    "Install Cartridge",
                    "Is the new cartridge installed?",
                    QMessageBox::Yes | QMessageBox::No
                );

                if (cartReply == QMessageBox::Yes) {
                    break; // Exit loop and continue
                }

                QMessageBox::information(
                    this,
                    "Reminder",
                    "Please install the new cartridge before proceeding."
                );
            }

            QProgressDialog *progress = new QProgressDialog("Detecting cartridge...", QString(), 0, 100, this);
            progress->setWindowTitle("Detecting Cartridge");
            progress->setCancelButton(nullptr);
            progress->setMinimumDuration(0);
            progress->setAutoClose(true);
            progress->setWindowModality(Qt::WindowModal);
            progress->show();

            int *value = new int(0);
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() mutable {
                (*value) += 10;
                progress->setValue(*value);
                if (*value >= 100) {
                    timer->stop();
                    delete timer;
                    delete value;

                    QMessageBox::information(this, "Cartridge Changed", "The cartridge has been successfully changed.");
                    QMessageBox::information(this, "Disconnect Tubing", "Make sure the set is disconnected from your body and securely connect the tubing to your cartridge.\n\nTap OK to fill your tubing with insulin.");
                    on_fillTubingButton_clicked();
                }
            });
            timer->start(300);
        };
        showInstallPrompt();
    });
}

// Fill Tubing
void optionsMenu::on_fillTubingButton_clicked()
{
    amountFilled = 0;
    isFilling = false;
    ui->insulinAmountLabel->setText("0.0 units");
    ui->startFillInsulinButton->setText("Start");
    ui->stackedWidget->setCurrentIndex(8);
}

void optionsMenu::on_startFillInsulinButton_clicked()
{
    static QTimer *progressTimer = nullptr;

    if (!isFilling) {
        QMessageBox *startingBox = new QMessageBox(this);
        startingBox->setWindowTitle("Filling");
        startingBox->setText("STARTING FILL...");
        startingBox->setStandardButtons(QMessageBox::NoButton);
        startingBox->show();

        QTimer::singleShot(3000, this, [=]() {
            startingBox->accept();

            ui->startFillInsulinButton->setText("Stop");
            ui->startFillInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
            isFilling = true;

            fillTimer = new QTimer(this);
            connect(fillTimer, &QTimer::timeout, this, [=]() mutable {
                amountFilled += 0.4;
                ui->insulinAmountLabel->setText(QString("%1 units").arg(amountFilled, 0, 'f', 1));
            });
            fillTimer->start(300);
        });
    } else {
        if (fillTimer) {
            fillTimer->stop();
            fillTimer->deleteLater();
            fillTimer = nullptr;
        }

        isFilling = false;

        QProgressDialog *progress = new QProgressDialog("Stopping fill...", QString(), 0, 100, this);
        progress->setCancelButton(nullptr);
        progress->setWindowModality(Qt::WindowModal);
        progress->setMinimumDuration(0);
        progress->setValue(0);
        progress->setWindowTitle("Stopping");

        int *val = new int(0);
        progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, this, [=]() mutable {
            *val += 10;
            progress->setValue(*val);

            if (*val >= 100) {
                progressTimer->stop();
                progressTimer->deleteLater();
                delete val;

                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Tubing Fill Complete");
                msgBox.setText(
                    "If you see drops at the end of the tubing, press DONE.\n"
                    "If no insulin drops are noticed, press FILL to continue filling.\n\n"
                    "Amount Filled: " + QString("%1 units").arg(amountFilled, 0, 'f', 1)
                );

                QPushButton *doneButton = msgBox.addButton("DONE", QMessageBox::AcceptRole);
                msgBox.addButton("FILL", QMessageBox::RejectRole);

                msgBox.exec();

                if (msgBox.clickedButton() == doneButton) {
                    ui->fillTubingButton->setEnabled(false);
                    ui->stackedWidget->setCurrentIndex(2);
                    amountFilled = 0.0;
                    ui->startFillInsulinButton->setText("Start");
                    ui->startFillInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
                    ui->insulinAmountLabel->setText("0.0 units");
                } else {
                    ui->insulinAmountLabel->setText(QString("%1 units").arg(amountFilled, 0, 'f', 1));
                    isFilling = true;

                    fillTimer = new QTimer(this);
                    connect(fillTimer, &QTimer::timeout, this, [=]() mutable {
                        amountFilled += 0.3;
                        ui->insulinAmountLabel->setText(QString("%1 units").arg(amountFilled, 0, 'f', 1));
                    });
                    fillTimer->start(300);
                }
            }
        });
        progressTimer->start(200);
    }
}

// Fill Cannula
void optionsMenu::on_fillCannulaButton_clicked()
{
    QMessageBox::StandardButton stopReply = QMessageBox::question(
        this,
        "Filling Cannula",
        "Insert a new set and connect to the filled tubing.\n\nOn the next screen, you will fill the cannula with insulin.",
        QMessageBox::Ok | QMessageBox::Cancel
    );
    if (stopReply != QMessageBox::Ok) return;

    bool ok = false;
    double fillAmount = QInputDialog::getDouble(
        this,
        "Cannula Fill Amount",
        "Enter amount of insulin to fill (units):",
        0.5,
        0.1,
        1.0,
        1,
        &ok
    );
    if (!ok) return;

    QMessageBox *fillBox = new QMessageBox(this);
    fillBox->setWindowTitle("Filling");
    fillBox->setText("STARTING FILL...");
    fillBox->setStandardButtons(QMessageBox::NoButton);
    fillBox->show();

    QTimer::singleShot(3000, this, [=]() {
        fillBox->accept();

        QMessageBox::information(this, "Fill Complete",
            QString("Cannula has been filled with %1 units of insulin.").arg(fillAmount, 0, 'f', 1));

        QMessageBox::StandardButton resumeReply = QMessageBox::question(
            this,
            "Resume Insulin Delivery",
            "Do you want to resume all insulin deliveries now?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (resumeReply == QMessageBox::Yes) {
            QMessageBox::information(this, "Resumed", "Insulin delivery has resumed.");
            ui->stopInsulinButton->setText("STOP INSULIN");
            ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");

        } else {
            QMessageBox::information(this, "Paused", "Insulin delivery remains paused.");
            ui->stopInsulinButton->setText("RESUME INSULIN");
            ui->stopInsulinButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        }

        if (Config::siteReminderEnabled) {
            QMessageBox::information(
                this,
                "Site Reminder Set",
                QString("Site reminder set for %1 days at %2.")
                    .arg(Config::siteReminderDays)
                    .arg(Config::siteReminderTime)
            );
        }

        QMessageBox::information(this, " ", "Test Blood Glucose in 1-2 hours.");
    });
}

// Site Reminder
void optionsMenu::updateReminderButtonLabel() {
    QString state = Config::siteReminderEnabled ? "ON" : "OFF";
    ui->siteReminderButton->setText(QString("Site Reminder: %1").arg(state));
}

void optionsMenu::on_siteReminderButton_clicked()
{
    int currentDays = Config::siteReminderDays > 0 ? Config::siteReminderDays : 3;
    QTime currentTime = QTime::fromString(Config::siteReminderTime, "HH:mm");
    if (!currentTime.isValid()) currentTime = QTime(12, 0);
    QString currentTimeStr = currentTime.toString("HH:mm");

    QString enabledStatus = Config::siteReminderEnabled ? "ON" : "OFF";
    QMessageBox::StandardButton toggle = QMessageBox::question(
        this,
        "Site Reminder",
        QString("Current reminder: %1 days at %2\nStatus: %3\n\nDo you want to change the settings?")
            .arg(currentDays)
            .arg(currentTimeStr)
            .arg(enabledStatus),
        QMessageBox::Yes | QMessageBox::No
    );
    if (toggle == QMessageBox::No) return;

    QMessageBox::StandardButton enableToggle = QMessageBox::question(
        this,
        "Reminder Enabled",
        QString("Site Reminder is currently %1.\n\nDo you want to turn it %2?")
            .arg(enabledStatus)
            .arg(Config::siteReminderEnabled ? "OFF" : "ON"),
        QMessageBox::Yes | QMessageBox::No
    );
    if (enableToggle == QMessageBox::Yes) {
        Config::siteReminderEnabled = !Config::siteReminderEnabled;
    }

    if (!Config::siteReminderEnabled) {
        QMessageBox::information(this, "Reminder Disabled", "Site reminder has been turned off.");
        updateReminderButtonLabel();
        return;
    }

    bool ok = false;
    int days = QInputDialog::getInt(
        this,
        "Remind Me In",
        "Enter number of days (1–3):",
        currentDays,
        1,
        3,
        1,
        &ok
    );
    if (!ok) return;

    int hours = QInputDialog::getInt(
        this,
        "Reminder Time",
        "Hour (0–23):",
        currentTime.hour(),
        0,
        23,
        1,
        &ok
    );
    if (!ok) return;

    int minutes = QInputDialog::getInt(
        this,
        "Reminder Time",
        "Minutes (0–59):",
        currentTime.minute(),
        0,
        59,
        1,
        &ok
    );
    if (!ok) return;

    QTime newTime(hours, minutes);
    QString timeStr = newTime.toString("HH:mm");

    QString confirmText = QString("Reminder will be set for %1 days at %2.\n\nConfirm?")
                            .arg(days)
                            .arg(timeStr);
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this,
        "Confirm Reminder",
        confirmText,
        QMessageBox::Yes | QMessageBox::No
    );
    if (confirm != QMessageBox::Yes) return;

    Config::siteReminderDays = days;
    Config::siteReminderTime = timeStr;

    QMessageBox::information(this, "Settings Saved", "Your site reminder has been saved.");

    updateReminderButtonLabel();
}

// Activity Menu
void optionsMenu::on_activityButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Programs On Exercise
void optionsMenu::on_programsOnExerciseButton_clicked()
{
    //Do Somethhing
}

// Sleep
void optionsMenu::on_sleepButton_clicked()
{
    //Do Something
}

// Temp Basal Rate
void optionsMenu::on_tempBasalRatesButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
    ui->tempRateButton->setEnabled(Config::tempBasalRateEnabled);
    ui->durationButton->setEnabled(Config::tempBasalRateEnabled);
}

void optionsMenu::on_tempBasalRateToggleButton_clicked()
{
    if (Config::controlIQEnabled) {
        QMessageBox::warning(this, "Not Allowed", "Please disable Control-IQ before enabling Temporary Basal Rate.");
        return;
    }

    Config::tempBasalRateEnabled = !Config::tempBasalRateEnabled;

    ui->tempBasalRateToggleButton->setText(Config::tempBasalRateEnabled ? "Temporary Basal Rate: ON" : "Temporary Basal Rate: OFF");
    ui->tempBasalRateToggleButton->setStyleSheet(Config::tempBasalRateEnabled
        ? "font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;"
        : "font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");

    ui->tempRateButton->setEnabled(Config::tempBasalRateEnabled);
    ui->durationButton->setEnabled(Config::tempBasalRateEnabled);
}

void optionsMenu::on_tempRateButton_clicked()
{
    bool ok;

    int rate = QInputDialog::getInt(
        this,
        "Set Temporary Basal Rate",
        "Enter desired rate percentage (0 - 250%):",
        100, 0, 250, 1, &ok);
    if (!ok) return;

    Config::instance()->setTempBasalRatePercentage(rate);
    ui->tempRateButton->setText(QString("%1%").arg(rate));
}

void optionsMenu::on_durationButton_clicked()
{
    bool ok;
    int hours = QInputDialog::getInt(this, "Set Duration", "Enter hours (0–72):", 0, 0, 72, 1, &ok);
    if (!ok) return;

    int minutes = QInputDialog::getInt(this, "Set Duration", "Enter minutes (0–59):", 15, 0, 59, 1, &ok);
    if (!ok) return;

    int totalMinutes = hours * 60 + minutes;

    if (totalMinutes < 15 || totalMinutes > 72 * 60) {
        QMessageBox::warning(this, "Invalid Duration", "Duration must be between 15 minutes and 72 hours.");
        return;
    }

    Config::instance()->setTempBasalDuration(totalMinutes);
    ui->durationButton->setText(QString("%1 hour(s) and %2 minute(s).").arg(hours).arg(minutes));
}

void optionsMenu::on_checkButton_TempBasalRate_clicked()
{
    int ratePercentage = Config::instance()->getTempBasalRatePercentage();
    int durationMinutes = Config::instance()->getTempBasalDuration();

    if (ratePercentage == -1 || durationMinutes == 0) {
        QMessageBox::warning(this, "Missing Info", "Please enter both temporary basal rate and duration.");
        return;
    }

    const double normalBasalRate = 1.0;
    const double minAllowedBasalRate = 0.1;
    const double maxAllowedBasalRate = 15.0;

    double programmedRate = normalBasalRate * (ratePercentage / 100.0);

    if (programmedRate < minAllowedBasalRate) {
        QMessageBox::information(
            this,
            "Rate Too Low",
            QString("The selected rate (%.2f u/hr) is below the minimum allowed.\nIt will be set to %.1f u/hr.")
                .arg(programmedRate)
                .arg(minAllowedBasalRate)
        );
        programmedRate = minAllowedBasalRate;
    }

    if (programmedRate > maxAllowedBasalRate) {
        QMessageBox::information(
            this,
            "Rate Too High",
            QString("The selected rate (%.2f u/hr) is too high.\nIt will be reduced to %.1f u/hr.")
                .arg(programmedRate)
                .arg(maxAllowedBasalRate)
        );
        programmedRate = maxAllowedBasalRate;
    }

    QMessageBox::information(
        this,
        "Temporary Basal Programmed",
        QString("Temp Basal Rate set to: %1% (%2 u/hr)\nDuration: %3 min")
            .arg(ratePercentage)
            .arg(programmedRate)
            .arg(durationMinutes)
    );

    ui->stackedWidget->setCurrentIndex(3);
}

// My Pump Menu
void optionsMenu::on_myPumpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

// Personal Profiles
void optionsMenu::on_personalProfilesButton_clicked()
{
    profilesPage->disableProfileButtons();
    profilesPage->loadProfilesFromFile();
    profilesPage->show();
}

// Control-IQ
void optionsMenu::on_controlIQButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->weightLabel->setVisible(Config::controlIQEnabled);
    ui->totalDailyInsulinLabel->setVisible(Config::controlIQEnabled);
    ui->weightButton->setVisible(Config::controlIQEnabled);
    ui->totalDailyInsulinButton->setVisible(Config::controlIQEnabled);
}

void optionsMenu::on_controlIQToggleButton_clicked()
{
    Config::controlIQEnabled = !Config::controlIQEnabled;

    ui->controlIQToggleButton->setText(Config::controlIQEnabled ? "Control-IQ: ON" : "Control-IQ: OFF");
    ui->controlIQToggleButton->setStyleSheet(Config::controlIQEnabled
        ? "font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;"
        : "font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");

    ui->weightLabel->setVisible(Config::controlIQEnabled);
    ui->totalDailyInsulinLabel->setVisible(Config::controlIQEnabled);
    ui->weightButton->setVisible(Config::controlIQEnabled);
    ui->totalDailyInsulinButton->setVisible(Config::controlIQEnabled);
}

void optionsMenu::on_weightButton_clicked()
{
    QStringList units = {"kg", "lbs"};
    bool ok = false;

    QString weightUnit = QInputDialog::getItem(
        this,
        "Select Unit",
        "Choose unit for weight:",
        units,
        0,
        false,
        &ok
    );

    if (!ok || weightUnit.isEmpty())
        return;

    bool okWeight = false;
    double min = (weightUnit == "kg") ? 25.0 : 55.0;      // minimum 25kg or 55lbs
    double max = (weightUnit == "kg") ? 140.0 : 308.0;    // maximum 140kg or 308lbs
    double defaultVal = (weightUnit == "kg") ? 70.0 : 150.0;

    double weight = QInputDialog::getDouble(
        this,
        "Enter Weight",
        QString("Enter your weight (%1):").arg(weightUnit),
        defaultVal,
        min,
        max,
        1,
        &okWeight
    );
    if (!okWeight) return;

    Config::instance()->setWeight(weight, weightUnit);
    ui->weightButton->setText(QString("%1 %2").arg(weight).arg(weightUnit));
}

void optionsMenu::on_totalDailyInsulinButton_clicked()
{
    bool ok = false;
    double insulin = QInputDialog::getDouble(
        this,
        "Total Daily Insulin",
        "Enter your total daily insulin (units):",
        50.0, 10.0, 100.0, 1, &ok);
    if (!ok) return;

    Config::instance()->setTotalDailyInsulin(insulin);
    ui->totalDailyInsulinButton->setText(QString("%1 u").arg(insulin));
}

void optionsMenu::on_checkButton_ControlIQ_clicked()
{
    double weight = Config::instance()->getWeight();
    double tdi = Config::instance()->getTotalDailyInsulin();

    if (weight == 0.0 || tdi == 0.0) {
        QMessageBox::warning(this, "Missing Info", "Please enter both weight and total daily insulin before continuing.");
        return;
    }

    QString units = Config::instance()->getWeightUnit();
    QMessageBox::information(
        this,
        "Control IQ Settings",
        QString("Weight: %1 %2\nTotal Daily Insulin: %3 units")
            .arg(weight)
            .arg(units)
            .arg(tdi)
    );

    Config::controlIQEnabled = true;
    emit controlIQStatusUpdated(true);
    emit basalDeliveryStatusUpdated(true);
    ui->stackedWidget->setCurrentIndex(4);
}

// Alerts and Reminders
void optionsMenu::on_alertsRemindersButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void optionsMenu::on_lowBGToggleButton_clicked()
{
    lowBGAlertEnabled = !lowBGAlertEnabled;

    if (lowBGAlertEnabled) {
        ui->lowBGToggleButton->setText("Low BG Alert: ON");
        ui->lowBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        ui->lowBGButton->setEnabled(true);
    } else {
        ui->lowBGToggleButton->setText("Low BG Alert: OFF");
        ui->lowBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
        ui->lowBGButton->setEnabled(false);
    }
}

void optionsMenu::on_lowBGButton_clicked()
{
    if (!lowBGAlertEnabled) return;

    bool ok = false;
    int lowBGValue = QInputDialog::getInt(
        this,
        "Remind Me Below",
        "Enter low blood glucose threshold (mg/dL):",
        70, 70, 120, 1, &ok);
    if (!ok) return;

    int repeatDelay = QInputDialog::getInt(
            this,
            "Remind Me After",
            "Remind me again after (minutes):",
            15, 10, 20, 1, &ok);
    if (!ok) return;


    QMessageBox::information(
        this,
        "Low BG Reminder Set",
        QString("Reminder set to alert below %1 mg/dL\nand repeat reminder after %2 minutes.")
            .arg(lowBGValue)
            .arg(repeatDelay)
    );

    Config::lowBGThreshold = lowBGValue;
    Config::lowBGRepeatDelay = repeatDelay;

    ui->lowBGButton->setText(QString("Low BG: %1 mg/dL, %2 min").arg(lowBGValue).arg(repeatDelay));
}

void optionsMenu::on_highBGToggleButton_clicked()
{
    highBGAlertEnabled = !highBGAlertEnabled;

    if (highBGAlertEnabled) {
        ui->highBGToggleButton->setText("High BG Alert: ON");
        ui->highBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        ui->highBGButton->setEnabled(true);
    } else {
        ui->highBGToggleButton->setText("High BG Alert: OFF");
        ui->highBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
        ui->highBGButton->setEnabled(false);
    }
}

void optionsMenu::on_highBGButton_clicked()
{
    if (!highBGAlertEnabled) return;

    bool ok = false;
    int highBGValue = QInputDialog::getInt(
        this,
        "Remind Me Above",
        "Enter high blood glucose threshold (mg/dL):",
        200, 150, 300, 1, &ok);
    if (!ok) return;

    int repeatDelay = QInputDialog::getInt(
        this,
        "Remind Me After",
        "Remind me again after (minutes):",
        120, 60, 180, 1, &ok);
    if (!ok) return;

    QMessageBox::information(
        this,
        "High BG Reminder Set",
        QString("Reminder set to alert above %1 mg/dL\nand repeat reminder after %2 minutes.")
            .arg(highBGValue)
            .arg(repeatDelay)
    );

    Config::highBGThreshold = highBGValue;
    Config::highBGRepeatDelay = repeatDelay;

    ui->highBGButton->setText(QString("High BG: %1 mg/dL, %2 min").arg(highBGValue).arg(repeatDelay));
}

void optionsMenu::on_afterBolusBGToggleButton_clicked()
{
    afterBolusBGAlertEnabled = !afterBolusBGAlertEnabled;

    if (afterBolusBGAlertEnabled) {
        ui->afterBolusBGToggleButton->setText("After Bolus BG Alert: ON");
        ui->afterBolusBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        ui->afterBolusBGButton->setEnabled(true);
    } else {
        ui->afterBolusBGToggleButton->setText("After Bolus BG Alert: OFF");
        ui->afterBolusBGToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
        ui->afterBolusBGButton->setEnabled(false);
    }
}

void optionsMenu::on_afterBolusBGButton_clicked()
{
    if (!afterBolusBGAlertEnabled) return;

    bool ok = false;
    int afterBolusMins = QInputDialog::getInt(
        this,
        "Repeat Reminder",
        "Remind me again after (minutes):",
        90, 60, 180, 1, &ok);
    if (!ok) return;

    QMessageBox::information(
        this,
        "After-Bolus BG Reminder Set",
        QString("Reminder to check BG in %1 minutes after bolus.")
            .arg(afterBolusMins)
    );

    Config::afterBolusTime = afterBolusMins;

    ui->afterBolusBGButton->setText(QString("After Bolus: %1 mins").arg(afterBolusMins));
}


void optionsMenu::on_missedMealToggleButton_clicked()
{
    missedMealAlertEnabled = !missedMealAlertEnabled;

    if (missedMealAlertEnabled) {
        ui->missedMealToggleButton->setText("Missed Meal Alert: ON");
        ui->missedMealToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        ui->missedMealBolusButton->setEnabled(true);
    } else {
        ui->missedMealToggleButton->setText("Missed Meal Alert: OFF");
        ui->missedMealToggleButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");
        ui->missedMealBolusButton->setEnabled(false);
    }
}

void optionsMenu::on_missedMealBolusButton_clicked()
{
    if (!missedMealAlertEnabled) return;

    bool okDays;
    QString daysInput = QInputDialog::getText(this, "Select Days",
                                              "Enter the days for the reminder (Mon, Tue, Wed, Thu, Fri, Sat, Sun):",
                                              QLineEdit::Normal, "", &okDays);

    if (!okDays || daysInput.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter at least one day.");
        return;
    }

    // Validate the entered days
    QStringList daysList = daysInput.split(",");
    QStringList validDays = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    for (int i = 0; i < daysList.size(); ++i) {
        daysList[i] = daysList[i].trimmed();
        if (!validDays.contains(daysList[i])) {
            QMessageBox::warning(this, "Invalid Day", QString("Invalid day entered: %1").arg(daysList[i]));
            return;
        }
    }

    bool okStartTime;
    QTime startTime = QTime::fromString("08:00", "hh:mm");
    startTime = QTime::fromString(QInputDialog::getText(this, "Start Time", "Select Start Time (HH:MM):", QLineEdit::Normal, startTime.toString("HH:mm"), &okStartTime));
    if (!okStartTime || !startTime.isValid()) {
        QMessageBox::warning(this, "Invalid Time", "Please enter a valid start time.");
        return;
    }

    bool okEndTime;
    QTime endTime = QTime::fromString("10:30", "hh:mm");
    endTime = QTime::fromString(QInputDialog::getText(this, "End Time", "Select End Time (HH:MM):", QLineEdit::Normal, endTime.toString("HH:mm"), &okEndTime));
    if (!okEndTime || !endTime.isValid() || endTime <= startTime) {
        QMessageBox::warning(this, "Invalid Time", "Please enter a valid end time.");
        return;
    }

    QString confirmText = QString("The Missed Meal Bolus reminder will be active for the following days:\n")
                          .append(daysList.join(", "))
                          .append(QString("\nStart Time: %1\nEnd Time: %2\n\nConfirm?").arg(startTime.toString("hh:mm")).arg(endTime.toString("hh:mm")));
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirm Missed Meal Reminder", confirmText, QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) {
        return;
    }

    Config::missedMealDays = daysList;
    Config::missedMealStartTime = startTime.toString("hh:mm");
    Config::missedMealEndTime = endTime.toString("hh:mm");

    ui->missedMealBolusButton->setText(QString("Missed Meal: %1 to %2,\nDays: %3")
                                       .arg(startTime.toString("hh:mm"))
                                       .arg(endTime.toString("hh:mm"))
                                       .arg(daysList.join(", ")));

    QMessageBox::information(this, "Settings Saved", "Your missed meal bolus reminder has been saved.");
}

// Pump Infoo
void optionsMenu::on_pumpInfoButton_clicked()
{
    QString pumpSerial = "SN315795";
    QString supportPhone = "1-877-801-6901";
    QString website = "www.tandemdiabetes.com";
    QString softwareVersion = "v7.4.0";
    QString hardwareVersion = "Tandem t:slim X2";

    QString message = QString(
        "<b>Pump Serial Number:</b> %1<br><br>"
        "<b>Customer Support:</b> %2<br>"
        "<b>Website:</b> <a href='http://%3'>%3</a><br><br>"
        "<b>Software Version:</b> %4<br>"
        "<b>Hardware Version:</b> %5"
    ).arg(pumpSerial, supportPhone, website, softwareVersion, hardwareVersion);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Pump Info");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

// My CGM
void optionsMenu::on_myCGMButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->calibrateCGMButton->setEnabled(false);
}

// Start Sensor
void optionsMenu::on_startStopSensorButton_clicked()
{
    if  (ui->startStopSensorButton->text() == "Start Sensor") {
        QMessageBox::information(this, "Start Sensor", "Starting new CGM session.");
        ui->calibrateCGMButton->setEnabled(true);
        ui->startStopSensorButton->setText("Stop Sensor");
        ui->startStopSensorButton->setStyleSheet("font: bold 12pt; background-color: #333; color: red; border-radius: 10px; border: 2px solid #555;");

    } else {
        QMessageBox::information(this, "Stop Sensor", "Stopping current CGM session.");
        ui->startStopSensorButton->setText("Start Sensor");
        ui->startStopSensorButton->setStyleSheet("font: bold 12pt; background-color: #333; color: green; border-radius: 10px; border: 2px solid #555;");
        ui->calibrateCGMButton->setEnabled(false);
    }
}

// Calibrate CGM
void optionsMenu::on_calibrateCGMButton_clicked()
{
    bool ok = false;
    int calibrationBG = QInputDialog::getInt(
       this,
       tr("Enter Blood Glucose"),
       tr("Enter the Blood Glucose value from the Blood Glucose Meter."),
       70,
       70,
       250,
       1,
       &ok
   );

    if (ok) {
        QString message = QString("Calibration Blood Glucose: %1 mg/dL").arg(calibrationBG);
        QMessageBox::information(this, "Calibration Accepted", message);
    }
}

void optionsMenu::on_cgmAlertsButton_clicked()
{
    //Do Something
}

// Transmitter ID
void optionsMenu::on_transmitterIDButton_clicked()
{
    bool ok = false;
    QString transmitterID = QInputDialog::getText(
        this,
        tr("Transmitter ID"),
        tr("Enter Transmitter ID:"),
        QLineEdit::Normal,
        "",
        &ok
    );

    if (ok && !transmitterID.trimmed().isEmpty()) {
        ui->transmitterIDButton->setText(QString("Transmitter ID: %1").arg(transmitterID.trimmed().toUpper()));
    }
}

// CGM Info
void optionsMenu::on_cgmInfoButton_clicked()
{
    QString cgmName = "Dexcom G6 CGM";
    QString firmwareRev = "1.0.4.10 Rev 02";
    QString hardwareRev = "Rev B - Model X2";
    QString hardwareID = "G6-5971365";
    QString softwareNumber = "v8.1.2 (Build 4123)";

    QString message = QString(
        "<b>CGM Name:</b> %1<br><br>"
        "<b>Firware Revision:</b> %2<br>"
        "<b>Hardware Revision:</b> %3<br><br>"
        "<b>Hardware ID:</b> %4<br>"
        "<b>Software Number:</b> %5"
    ).arg(cgmName, firmwareRev, hardwareRev, hardwareID, softwareNumber);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Pump Info");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

// Device Settings Menu
void optionsMenu::on_deviceSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

// Display Settingss
void optionsMenu::on_displaySettingsButton_clicked()
{
    bool ok = false;
    int timeout = QInputDialog::getInt(
        this,
        "Screen Timeout",
        "Set screen timeout (seconds):",
        30, 5, 120, 1, &ok);

    if (ok) {
        QMessageBox::information(this, "Settings Saved",
            QString("Screen timeout has been set to %1 seconds.").arg(timeout));
    }
}

// Sound Volume
void optionsMenu::on_soundVolumeButton_clicked()
{
    QComboBox *soundComboBox = new QComboBox(this);
    soundComboBox->addItem("High");
    soundComboBox->addItem("Medium");
    soundComboBox->addItem("Low");
    soundComboBox->addItem("Vibrate");

    QDialog *soundDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Select sound volume:"));
    layout->addWidget(soundComboBox);

    QPushButton *confirmButton = new QPushButton("Confirm", soundDialog);
    layout->addWidget(confirmButton);

    soundDialog->setLayout(layout);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        QString selectedSound = soundComboBox->currentText();

        QMessageBox::information(this, "Sound Volume",
            QString("Sound volume has been set to %1.").arg(selectedSound));

        Config::soundVolume = selectedSound;
        ui->soundVolumeButton->setText("Sound Volume: " + selectedSound);
        soundDialog->accept();
    });

    soundDialog->exec();
}

// Time & Date
void optionsMenu::on_timeDateButton_clicked()
{
    QDialog *settingsDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout;

    QComboBox *timeFormatComboBox = new QComboBox(settingsDialog);
    timeFormatComboBox->addItem("24-hour");
    timeFormatComboBox->addItem("12-hour");
    timeFormatComboBox->setCurrentText(Config::timeFormat == "24" ? "24-hour" : "12-hour");

    QComboBox *dateFormatComboBox = new QComboBox(settingsDialog);
    dateFormatComboBox->addItem("Day Month");
    dateFormatComboBox->addItem("Day/Month/Year");
    dateFormatComboBox->setCurrentText(Config::dateFormat == "dd MMM" ? "Day Month" : "Day/Month/Year");

    layout->addWidget(new QLabel("Select Time Format:"));
    layout->addWidget(timeFormatComboBox);
    layout->addWidget(new QLabel("Select Date Format:"));
    layout->addWidget(dateFormatComboBox);

    QPushButton *confirmButton = new QPushButton("Confirm", settingsDialog);
    layout->addWidget(confirmButton);

    settingsDialog->setLayout(layout);

    connect(confirmButton, &QPushButton::clicked, this, [=]() {
        Config::timeFormat = (timeFormatComboBox->currentText() == "24-hour") ? "24" : "12";
        Config::dateFormat = (dateFormatComboBox->currentText() == "Day Month") ? "dd MMM" : "dd/MM/yyyy";

        QMessageBox::information(this, "Settings Updated", "Time and Date formats have been updated.");
        settingsDialog->accept();
    });

    settingsDialog->exec();
}

// Bluetooth Settings
void optionsMenu::on_bluetoothSettingsButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Bluetooth Settings",
        bluetoothEnabled ? "Do you want to turn Bluetooth off?" : "Do you want to turn Bluetooth on?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        bluetoothEnabled = !bluetoothEnabled;
        ui->bluetoothSettingsButton->setText(bluetoothEnabled ? "Bluetooth Settings: ON": "Bluetooth Settings: OFF");
    }
}

// History Menu
void optionsMenu::on_historyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

// Pump History
void optionsMenu::on_pumpHistoryButton_clicked()
{
    //Do Something
}

// Bolus History
void optionsMenu::on_bolusHistoryButton_clicked()
{
    //Do Something
}

// Control-IQ History
void optionsMenu::on_controlIQHistoryButton_clicked()
{
    //Do Something
}

// Back Buttons
void optionsMenu::on_backButton_OptionsMain_clicked()
{
    emit returnToMainWindow();
    this->close();
}

void optionsMenu::on_backButton_Load_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->tandemLogoButton->setEnabled(true);
}

void optionsMenu::on_backButton_Activity_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_backButton_MyPump_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_backButton_MyCGM_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_backButton_DeviceSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_backButton_History_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void optionsMenu::on_backButton_AlertsReminders_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void optionsMenu::on_backButton_ControlIQ_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void optionsMenu::on_backButton_TempBasalRate_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
