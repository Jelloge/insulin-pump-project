#include "personalprofiles.h"
#include "ui_personalprofiles.h"
#include "batterymanager.h"
#include "mainwindow.h"
#include "config.h"

personalProfiles::personalProfiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::personalProfiles),

    newProfile(new profile()),
    newTimeProfile(new timeProfile())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Back Buttons
    QPixmap backPixmap(":/ui_icons/back.png");
    QIcon backIcon(backPixmap);

    ui->backButton_ProfileMain->setText("");
    ui->backButton_ProfileMain->setIcon(backIcon);
    ui->backButton_ProfileMain->setIconSize(QSize(55, 55));

    ui->backButton_Keyboard->setText("");
    ui->backButton_Keyboard->setIcon(backIcon);
    ui->backButton_Keyboard->setIconSize(QSize(55, 55));

    ui->backButton_ProfileSetUp->setText("");
    ui->backButton_ProfileSetUp->setIcon(backIcon);
    ui->backButton_ProfileSetUp->setIconSize(QSize(55, 55));

    ui->backButton_TimedSettings->setText("");
    ui->backButton_TimedSettings->setIcon(backIcon);
    ui->backButton_TimedSettings->setIconSize(QSize(55, 55));

    ui->backButton_BolusSettings->setText("");
    ui->backButton_BolusSettings->setIcon(backIcon);
    ui->backButton_BolusSettings->setIconSize(QSize(55, 55));

    // Add Button
    QPixmap plusPixmap(":/ui_icons/plus.png");
    QIcon plusIcon(plusPixmap);

    ui->addProfileButton->setText("");
    ui->addProfileButton->setIcon(plusIcon);
    ui->addProfileButton->setIconSize(QSize(22, 22));

    // Check Buttons
    QPixmap checkPixmap(":/ui_icons/check.png");
    QIcon checkIcon(checkPixmap);

    ui->checkButton->setText("");
    ui->checkButton->setIcon(checkIcon);
    ui->checkButton->setIconSize(QSize(25, 25));

    ui->checkButton_Bolus->setText("");
    ui->checkButton_Bolus->setIcon(checkIcon);
    ui->checkButton_Bolus->setIconSize(QSize(25, 25));

    ui->checkButton_Timed->setText("");
    ui->checkButton_Timed->setIcon(checkIcon);
    ui->checkButton_Timed->setIconSize(QSize(25, 25));

    ui->checkButton_TimedBolus->setText("");
    ui->checkButton_TimedBolus->setIcon(checkIcon);
    ui->checkButton_TimedBolus->setIconSize(QSize(25, 25));

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

    // Connecting letter and number buttons
    connect(ui->aButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->bButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->cButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->dButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->eButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->gButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->hButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->iButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->jButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->kButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->lButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->mButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->nButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->oButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->pButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->qButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->rButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->tButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->uButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->vButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->wButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->xButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->yButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->zButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->oneButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->twoButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->threeButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fourButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fiveButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sixButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sevenButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->eightButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->nineButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->zeroButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);

    // Connecting space and delete buttons
    connect(ui->spaceButton, &QPushButton::clicked, this, [=]() {
        ui->profileNameEdit->insert(" ");
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {
        QString text = ui->profileNameEdit->text();
        text.chop(1);
        ui->profileNameEdit->setText(text);
    });

    // Connecting activate, edit, duplicate and delete buttons
    connect(ui->profileListWidget, &QListWidget::itemClicked,
            this, &personalProfiles::on_profileListWidget_itemClicked);

}

personalProfiles::~personalProfiles()
{
    delete newProfile;
    delete ui;
}

QString personalProfiles::getProfilesFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles.json";
}

QJsonArray personalProfiles::loadProfiles()
{
    QString filePath = getProfilesFilePath();
    QFile file(filePath);

    if (!file.exists()) {
        return {};
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open profiles.json");
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.isArray() ? doc.array() : QJsonArray();
}

int personalProfiles::findProfileIndex(const QString &name, const QJsonArray &profiles)
{
    for (int i = 0; i < profiles.size(); ++i) {
        QJsonObject obj = profiles[i].toObject();
        if (obj["name"].toString() == name) return i;
    }
    return -1;
}

bool personalProfiles::saveProfiles(const QJsonArray &profiles)
{
    QFile file(getProfilesFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    file.write(QJsonDocument(profiles).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void personalProfiles::loadProfilesFromFile()
{
    QJsonArray profiles = loadProfiles();
    ui->profileListWidget->clear();
    activeProfileName.clear();

    for (const QJsonValue &val : profiles) {
        QJsonObject obj = val.toObject();
        QString name = obj["name"].toString();
        bool isActive = obj["isActive"].toBool();

        if (isActive) {
            activeProfileName = name;
        }

        QListWidgetItem *item = new QListWidgetItem(name);
        if (isActive) {
            QFont font = item->font();
            font.setPointSize(14);
            font.setBold(true);
            item->setFont(font);
        }
        ui->profileListWidget->addItem(item);
    }
}

void personalProfiles::disableProfileButtons()
{
    ui->activateProfileButton->setEnabled(false);
    ui->editProfileButton->setEnabled(false);
    ui->duplicateProfileButton->setEnabled(false);
    ui->deleteProfileButton->setEnabled(false);
    selectedProfileName.clear();
}

void personalProfiles::on_tandemLogoButton_clicked()
{
    emit returnToMain();
    this->hide();
}

void personalProfiles::on_addProfileButton_clicked()
{
    if (newProfile) delete newProfile;
    newProfile = new profile();
    ui->profileNameEdit->setText("");
    ui->stackedWidget->setCurrentIndex(1);
}

void personalProfiles::on_virtualKeyPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
       if (!button) return;

       QString key = button->text();
       ui->profileNameEdit->insert(key);
}

void personalProfiles::on_checkButton_clicked()
{
    QString profileName = ui->profileNameEdit->text().trimmed();

    if (profileName.isEmpty()) {
            QMessageBox::warning(this, "Missing Data", "Please enter a profile name.");
            return;
    }

    newProfile->profileName = profileName;
    ui->currProfileNameLabel->setText(profileName);
    ui->stackedWidget->setCurrentIndex(2);
}

void personalProfiles::setProfileSetUpPage()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void personalProfiles::on_timedSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void personalProfiles::on_bolusSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void personalProfiles::on_basalRateButton_clicked()
{
    bool ok = false;
    double basalRate = QInputDialog::getDouble(
        this,
        tr("Enter Basal Rate"),
        tr("Range: 0.1 to 15.0 units/hr"),
        0.1,        // initial
        0.1,        // min
        15.0,       // max
        1,          // decimals
        &ok
    );

    if (ok) {
        newProfile->basalRate = basalRate;
        ui->basalRateButton->setText(QString::number(basalRate, 'f', 1) + " units/hr");
    }
}

void personalProfiles::on_correctionFactorButton_clicked()
{
    bool ok = false;
    int corrFact = QInputDialog::getInt(
       this,
       tr("Enter Correction Factor"),
       tr("Range: 1 unit:1 mg/dL to 1 unit:600 mg/dL"),
       1,       // initial
       1,       // min
       600,     // max
       1,       // step
       &ok
   );

    if (ok) {
        newProfile->correctionFactor = corrFact;
        ui->correctionFactorButton->setText(QString("1 unit:%1 mg/dL").arg(corrFact));
    }
}

void personalProfiles::on_carbRatioButton_clicked()
{
    bool ok = false;
    int carbRatio = QInputDialog::getInt(
       this,
       tr("Enter Carb Ratio"),
       tr("Range: 1 unit:1 gram to 1 unit:300 grams"),
       1,
       1,
       300,
       1,
       &ok
   );

    if (ok) {
        newProfile->carbRatio = carbRatio;
        ui->carbRatioButton->setText(QString("1 unit:%1 g").arg(carbRatio));
    }
}

void personalProfiles::on_targetBGButton_clicked()
{
    bool ok = false;
    int targetBG = QInputDialog::getInt(
       this,
       tr("Enter Target Blood Glucose"),
       tr("Range: 70  mg/dL to 250 mg/dL"),
       70,
       70,
       250,
       1,
       &ok
   );

    if (ok) {
        newProfile->targetBG = targetBG;
        ui->targetBGButton->setText(QString("%1 mg/dL").arg(targetBG));
    }
}

void personalProfiles::on_checkButton_Timed_clicked()
{
    double br     = newProfile->basalRate;
    int    cF     = newProfile->correctionFactor;
    int    cR     = newProfile->carbRatio;
    int    tBG    = newProfile->targetBG;

    if (br <= 0 || cF <= 0 || cR <= 0 || tBG <= 0) {
        QMessageBox::warning(this, "Missing Data", "Please fill in all fields.");
        return;
    }

    QString timeToUse = editingProfileTime.isEmpty() ? "12:00" : editingProfileTime;

    QJsonObject timeProfileObj;
    timeProfileObj["profileTime"] = timeToUse;
    timeProfileObj["basalRate"] = br;
    timeProfileObj["correctionFactor"] = cF;
    timeProfileObj["carbRatio"] = cR;
    timeProfileObj["targetBG"] = tBG;
    timeProfileObj["isActive"] = true;

    QString msg = QString("Basal Rate: %1 units/hr\n"
                          "Correction Factor: 1 unit:%2 mg/dL\n"
                          "Carb Ratio: 1 unit:%3 g\n"
                          "Target BG: %4 mg/dL")
                    .arg(br, 0 , 'f', 1)
                    .arg(cF)
                    .arg(cR)
                    .arg(tBG);


    QMessageBox::StandardButton reply = QMessageBox::information(
        this,
        "Confirm Settings?",
        msg,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Ok) {
        QString topLine = QString("%1%2%3%4%5")
            .arg(QString("%1 u/hr").arg(br, 0, 'f', 1).leftJustified(20))
            .arg(QString("1u:%1 mg/dL").arg(cF).leftJustified(20))
            .arg(QString(" "), 10)
            .arg(QString("1u:%1 g").arg(cR).leftJustified(20))
            .arg(QString("%1 mg/dL").arg(tBG).leftJustified(17));

        QString labelLine = QString("%1%2%3%4%5")
            .arg("   BASAL", -17)
            .arg("   CORR FACTOR", -19)
            .arg("", -5)
            .arg("   CARB RATIO", -15)
            .arg("TARGET BG", 15);
        QString buttonText = topLine + "\n" + labelLine;

        ui->timedSettingsButton->setText(buttonText);
        emit addAndSwitchToTimedSettingsPage(timeProfileObj);
        editingProfileTime.clear();
    }
}

void personalProfiles::on_insulinDurationButton_clicked()
{
    bool ok = false;
    int hours = QInputDialog::getInt(
        this,
        "Insulin Duration",
        "Hours (2–8):",
        5,
        2,
        8,
        1,
        &ok);
    if (!ok) return;

    int minutes = QInputDialog::getInt(
        this,
        "Insulin Duration",
        "Minutes:",
        0,
        0,
        59,
        1,
        &ok);
    if (!ok) return;

    int totalMinutes = hours * 60 + minutes;
    newProfile->insulinDuration = totalMinutes;
    ui->insulinDurationButton->setText(QString("%1h %2m").arg(hours).arg(minutes));
}

void personalProfiles::on_maxBolusButton_clicked()
{
    bool ok = false;
    int maxBolus = QInputDialog::getInt(
       this,
       tr("Enter Max Bolus"),
       tr("Range: 1 to 25 units"),
       10,
       1,
       25,
       1,
       &ok
   );

    if (ok) {
        newProfile->maxBolus = maxBolus;
        ui->maxBolusButton->setText(QString("%1 units").arg(maxBolus));
    }
}

void personalProfiles::on_checkButton_Bolus_clicked()
{
    int totalMinutes    = newProfile->insulinDuration;
    int hr              = totalMinutes / 60;
    int min             = totalMinutes % 60;
    int mB              = newProfile->maxBolus;
    bool carbsOn        = newProfile->carbsEnabled;

    if (totalMinutes <= 0 || mB <= 0) {
        QMessageBox::warning(this, "Missing Data", "Please fill in all fields.");
        return;
    }

    QString msg = QString("Insulin Duration: %1h %2m\n"
                         "Max Bolus: %3 units\n"
                         "Carbohydrates: %4 ")
                    .arg(hr)
                    .arg(min)
                    .arg(mB)
                    .arg(carbsOn ? "On" : "Off");


    QMessageBox::StandardButton reply = QMessageBox::information(
        this,
        "Confirm Settings?",
        msg,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Ok) {
        QString topLine = QString("%1%2%3%4%5")
            .arg(QString("%1h %2m").arg(hr).arg(min, 2, 10, QChar('0')), -18)
            .arg(QString(" "), -10)
            .arg(QString("%1 units").arg(mB), -18)
            .arg(QString(" "), -14)
            .arg(carbsOn ? "ON" : "OFF");

        QString labelLine = QString("%1%2%3%4%5")
            .arg("   DURATION", -16)
            .arg("", -10)
            .arg(" MAX BOLUS", -18)
            .arg("", -10)
            .arg("CARBS");
        QString buttonText = topLine + "\n" + labelLine;

        ui->bolusSettingsButton->setText(buttonText);
        ui->stackedWidget->setCurrentIndex(2);
    }
}

void personalProfiles::on_carbohydratesCheckBox_stateChanged(int state)
{
    if (!newProfile) return;

    if (state == Qt::Checked) {
        newProfile->carbsEnabled = true;
    } else {
        newProfile->carbsEnabled = false;
    }
}

void personalProfiles::on_checkButton_TimedBolus_clicked()
{
    QString name            = newProfile->profileName;
    double  br              = newProfile->basalRate;
    int     cF              = newProfile->correctionFactor;
    int     cR              = newProfile->carbRatio;
    int     tBG             = newProfile->targetBG;
    int     totalMinutes    = newProfile->insulinDuration;
    int     hr              = totalMinutes / 60;
    int     min             = totalMinutes % 60;
    int     mB              = newProfile->maxBolus;
    bool    carbsOn         = newProfile->carbsEnabled;

    if (name.isEmpty() || br <= 0 || cF <= 0 || cR <= 0 || tBG <= 0 || totalMinutes <= 0 || mB <= 0) {
        QMessageBox::warning(this, "Missing Data", "Please fill in all fields.");
        return;
    }

    QString confirmMsg = QString(
        "Profile Name: %1\n\n"
        "➤ Timed Settings\n"
        "  • Basal Rate:       %2 u/hr\n"
        "  • Corr. Factor:      1u:%3 mg/dL\n"
        "  • Carb Ratio:       1u:%4 g\n"
        "  • Target BG:        %5 mg/dL\n\n"
        "➤ Bolus Settings\n"
        "  • Insulin Duration:  %6h %7m\n"
        "  • Max Bolus:           %8 units\n"
        "  • Carbohydrates:    %9"
    )
    .arg(name)
    .arg(QString::number(br, 'f', 1))
    .arg(cF)
    .arg(cR)
    .arg(tBG)
    .arg(hr)
    .arg(min, 2, 10, QChar('0')) // pad minutes
    .arg(mB)
    .arg(carbsOn ? "On" : "Off");

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Profile Settings",
        confirmMsg,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (reply != QMessageBox::Ok) return;

    QJsonObject profileObj;
    profileObj["name"] = name;
    profileObj["basalRate"] = br;
    profileObj["correctionFactor"] = cF;
    profileObj["carbRatio"] = cR;
    profileObj["targetBG"] = tBG;
    profileObj["insulinDuration"] = totalMinutes;
    profileObj["maxBolus"] = mB;
    profileObj["carbsEnabled"] = carbsOn;
    profileObj["isActive"] = (selectedProfileName  == activeProfileName);

    QJsonArray profilesArray = loadProfiles();
    int index = findProfileIndex(selectedProfileName, profilesArray);

    for (int i = 0; i < profilesArray.size(); ++i) {
        QJsonObject obj = profilesArray[i].toObject();
        QString existingName = obj["name"].toString();

        if (existingName == name && existingName != selectedProfileName) {
            QMessageBox::warning(this, "Duplicate Name",
                                 "A profile with this name already exists.");
            return;
        }
    }

    bool replaced = false;
    if (index != -1) {
        profilesArray[index] = profileObj;
        replaced = true;
    } else {
        profilesArray.append(profileObj);
    }

    if (!saveProfiles(profilesArray)) {
        QMessageBox::warning(this, "Error", "Failed to save profile.");
        return;
    }

    QMessageBox::information(this, "Profile Saved",
                             replaced ? "Profile updated successfully." : "New profile saved.");

    loadProfilesFromFile();
    delete newProfile;
    newProfile = nullptr;
    selectedProfileName.clear();

    ui->stackedWidget->setCurrentIndex(0);
}

// Back Buttons
void personalProfiles::on_backButton_ProfileMain_clicked()
{
    emit returnToOptions();
    this->close();
}
void personalProfiles::on_backButton_Keyboard_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void personalProfiles::on_backButton_ProfileSetUp_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void personalProfiles::on_backButton_TimedSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void personalProfiles::on_backButton_BolusSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Activate, Edit, Duplicate, Delete Profile
void personalProfiles::on_profileListWidget_itemClicked(QListWidgetItem *item)
{
    if (!item) return;

    selectedProfileName = item->text();
    bool isActive = (selectedProfileName == activeProfileName);

    ui->activateProfileButton->setEnabled(!isActive);
    ui->editProfileButton->setEnabled(true);
    ui->duplicateProfileButton->setEnabled(true);
    ui->deleteProfileButton->setEnabled(!isActive);
    ui->profileListWidget->setCurrentItem(item);
}

void personalProfiles::on_deleteProfileButton_clicked()
{
    if (selectedProfileName.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a profile to delete.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        "Delete Profile: " + selectedProfileName + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) return;

    QJsonArray profiles = loadProfiles();
    int index = findProfileIndex(selectedProfileName, profiles);
    if (index == -1) {
        QMessageBox::warning(this, "Not Found", "Profile not found.");
        return;
    }

    profiles.removeAt(index);

    if (!saveProfiles(profiles)) {
        QMessageBox::warning(this, "Error", "Failed to save.");
        return;
    }

    loadProfilesFromFile();
    selectedProfileName.clear();

    QMessageBox::information(this, "Deleted", "Profile deleted.");
}

void personalProfiles::on_editProfileButton_clicked()
{
    if (selectedProfileName.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a profile to edit.");
        return;
    }

    QJsonArray profiles = loadProfiles();
    int index = findProfileIndex(selectedProfileName, profiles);
    if (index == -1) {
        QMessageBox::warning(this, "Not Found", "Profile not found.");
        return;
    }

    QJsonObject obj = profiles[index].toObject();
    bool wasActive = obj["isActive"].toBool();

    if (newProfile) delete newProfile;
    newProfile = new profile();

    newProfile->profileName     = obj["name"].toString();
    newProfile->basalRate       = obj["basalRate"].toDouble();
    newProfile->correctionFactor = obj["correctionFactor"].toInt();
    newProfile->carbRatio       = obj["carbRatio"].toInt();
    newProfile->targetBG        = obj["targetBG"].toInt();
    newProfile->insulinDuration  = obj["insulinDuration"].toInt();
    newProfile->maxBolus        = obj["maxBolus"].toInt();
    newProfile->carbsEnabled    = obj["carbsEnabled"].toBool();
    newProfile->isActive        = wasActive;

    if (wasActive) {
        activeProfileName = newProfile->profileName;
    }

    int totalMin = newProfile->insulinDuration;
    int hr = totalMin / 60;
    int min = totalMin % 60;

    ui->profileNameEdit->setText(newProfile->profileName);

    // Timed Settings
    QString timedTop = QString("%1%2%3%4%5")
        .arg(QString("%1 u/hr").arg(newProfile->basalRate, 0, 'f', 1).leftJustified(20))
        .arg(QString("1u:%1 mg/dL").arg(newProfile->correctionFactor).leftJustified(20))
        .arg(QString(" "), 10)
        .arg(QString("1u:%1 g").arg(newProfile->carbRatio).leftJustified(20))
        .arg(QString("%1 mg/dL").arg(newProfile->targetBG).leftJustified(17));

    QString timedBottom = QString("%1%2%3%4%5")
        .arg("   BASAL", -17)
        .arg("   CORR FACTOR", -19)
        .arg("", -5)
        .arg("   CARB RATIO", -15)
        .arg("TARGET BG", 15);

    ui->timedSettingsButton->setText(timedTop + "\n" + timedBottom);
    ui->basalRateButton->setText(QString::number(newProfile->basalRate, 'f', 1) + " units/hr");
    ui->correctionFactorButton->setText(QString("1 unit:%1 mg/dL").arg(newProfile->correctionFactor));
    ui->carbRatioButton->setText(QString("1 unit:%1 g").arg(newProfile->carbRatio));
    ui->targetBGButton->setText(QString("%1 mg/dL").arg(newProfile->targetBG));

    // Bolus Settings
    QString bolusTop = QString("%1%2%3%4%5")
        .arg(QString("%1h %2m").arg(hr).arg(min, 2, 10, QChar('0')), -18)
        .arg(QString(" "), -10)
        .arg(QString("%1 units").arg(newProfile->maxBolus), -18)
        .arg(QString(" "), -14)
        .arg(newProfile->carbsEnabled ? "ON" : "OFF");

    QString bolusBottom = QString("%1%2%3%4%5")
        .arg("   DURATION", -16)
        .arg("", -10)
        .arg(" MAX BOLUS", -18)
        .arg("", -10)
        .arg("CARBS");

    ui->bolusSettingsButton->setText(bolusTop + "\n" + bolusBottom);
    ui->insulinDurationButton->setText(QString("%1h %2m").arg(hr).arg(min));
    ui->maxBolusButton->setText(QString("%1 units").arg(newProfile->maxBolus));
    ui->carbohydratesCheckBox->setChecked(newProfile->carbsEnabled);
    ui->stackedWidget->setCurrentIndex(1);
}

void personalProfiles::on_duplicateProfileButton_clicked()
{
    if (selectedProfileName.isEmpty()) {
           QMessageBox::warning(this, "No Selection", "Please select a profile to duplicate.");
           return;
       }

       QJsonArray profiles = loadProfiles();
       int index = findProfileIndex(selectedProfileName, profiles);
       if (index == -1) {
           QMessageBox::warning(this, "Not Found", "Profile not found.");
           return;
       }

       QJsonObject originalProfile = profiles[index].toObject();

       bool ok = false;
       QString newName = QInputDialog::getText(
           this,
           "Duplicate Profile",
           "Enter new profile name:",
           QLineEdit::Normal,
           selectedProfileName + " - Copy",
           &ok
       );

       if (!ok || newName.trimmed().isEmpty()) return;

       for (const QJsonValue &val : profiles) {
           if (val.toObject()["name"].toString() == newName.trimmed()) {
               QMessageBox::warning(this, "Duplicate Name", "A profile with that name already exists.");
               return;
           }
       }

       originalProfile["name"] = newName.trimmed();
       originalProfile["isActive"] = false;
       profiles.append(originalProfile);

       if (!saveProfiles(profiles)) {
           QMessageBox::warning(this, "Error", "Failed to save duplicated profile.");
           return;
       }

       loadProfilesFromFile();
       QMessageBox::information(this, "Success", "Profile duplicated as: " + newName);
}

void personalProfiles::on_activateProfileButton_clicked()
{
    if (selectedProfileName.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a profile to activate.");
        return;
    }
    if (selectedProfileName == activeProfileName) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Activation",
        "Activate Profile: " + selectedProfileName + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) return;

    QJsonArray profiles = loadProfiles();
    bool found = false;
    for (int i = 0; i < profiles.size(); ++i) {    // Set isActive false for all, true for selected
        QJsonObject obj = profiles[i].toObject();
        if (obj["name"].toString() == selectedProfileName) {
            obj["isActive"] = true;
            activeProfileName = selectedProfileName;
            found = true;
        } else {
            obj["isActive"] = false;
        }
        profiles[i] = obj;
    }

    if (!found) {
        QMessageBox::warning(this, "Error", "Profile already activated.");
        return;
    }

    if (!saveProfiles(profiles)) {
        QMessageBox::warning(this, "Error", "Failed to activate profile.");
        return;
    }

    QMessageBox::information(this, "Activated", "Activated Profile: " + selectedProfileName);
    loadProfilesFromFile();
    ui->activateProfileButton->setEnabled(false);
    ui->deleteProfileButton->setEnabled(false);
}

void personalProfiles::handleNewTimeSegment(QJsonObject segment, int targetPageIndex)
{
    editingProfileTime = segment["profileTime"].toString();

    double br    = segment["basalRate"].toDouble();
    int cF       = segment["correctionFactor"].toInt();
    int cR       = segment["carbRatio"].toInt();
    int tBG      = segment["targetBG"].toInt();

    ui->profileTimeLabel->setText(editingProfileTime);

    QString topLine = QString("%1%2%3%4%5")
        .arg(QString("%1 u/hr").arg(br, 0, 'f', 1).leftJustified(20))
        .arg(QString("1u:%1 mg/dL").arg(cF).leftJustified(20))
        .arg(QString(" "), 10)
        .arg(QString("1u:%1 g").arg(cR).leftJustified(20))
        .arg(QString("%1 mg/dL").arg(tBG).leftJustified(17));

    QString labelLine = QString("%1%2%3%4%5")
        .arg("   BASAL", -17)
        .arg("   CORR FACTOR", -19)
        .arg("", -5)
        .arg("   CARB RATIO", -15)
        .arg("TARGET BG", 15);

    ui->timedSettingsButton->setText(topLine + "\n" + labelLine);

    newProfile->basalRate = br;
    newProfile->correctionFactor = cF;
    newProfile->carbRatio = cR;
    newProfile->targetBG = tBG;

    ui->stackedWidget->setCurrentIndex(targetPageIndex);
}

