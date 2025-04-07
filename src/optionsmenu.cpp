#include "optionsmenu.h"
#include "ui_optionsmenu.h"

optionsMenu::optionsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::optionsMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Back Button
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
    connect(ui->ChargeButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
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

// Stop Insulin
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

// Load
void optionsMenu::on_loadButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->tandemLogoButton->setEnabled(false);
}

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
    });
}

// Activity
void optionsMenu::on_activityButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Fixes issue where Tandom Logo Button breaks battery and power buttons !
// void optionsMenu::on_tandemLogoButton_clicked()
// {
//     if (QWidget *parentWindow = this->parentWidget()) {
//         if (auto *main = qobject_cast<MainWindow *>(parentWindow)) {
//             main->refreshBatteryBindings();  // 🔁 Rebind labels
//         }

//         this->hide();
//         parentWindow->show();
//     }
// }


// My Pump
void optionsMenu::on_myPumpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void optionsMenu::on_personalProfilesButton_clicked()
{
    profilesPage->disableProfileButtons();
    profilesPage->loadProfilesFromFile();
    profilesPage->show();
}

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

void optionsMenu::on_deviceSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void optionsMenu::on_historyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void optionsMenu::on_backButton_OptionsMain_clicked()
{
    emit returnToMainWindow();
    this->close();
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



