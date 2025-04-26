#include "timedsettingsprofiles.h"
#include "ui_timedsettingsprofiles.h"

timedSettingsProfiles::timedSettingsProfiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::timedSettingsProfiles),
    newTimeProfile(new timeProfile())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Back Buttons
    QPixmap backPixmap(":/ui_icons/back.png");
    QIcon backIcon(backPixmap);

    ui->backButton_TSMenu->setText("");
    ui->backButton_TSMenu->setIcon(backIcon);
    ui->backButton_TSMenu->setIconSize(QSize(55, 55));

    ui->backButton_TimeSegment->setText("");
    ui->backButton_TimeSegment->setIcon(backIcon);
    ui->backButton_TimeSegment->setIconSize(QSize(55, 55));

    // Add Button
    QPixmap plusPixmap(":/ui_icons/plus.png");
    QIcon plusIcon(plusPixmap);

    ui->addTSButton->setText("");
    ui->addTSButton->setIcon(plusIcon);
    ui->addTSButton->setIconSize(QSize(22, 22));

    // Check Button
    QPixmap checkPixmap(":/ui_icons/check.png");
    QIcon checkIcon(checkPixmap);

    ui->checkButton_TimeSegment->setText("");
    ui->checkButton_TimeSegment->setIcon(checkIcon);
    ui->checkButton_TimeSegment->setIconSize(QSize(25, 25));

    ui->timeOfDayButton->setText("AM");
    ui->timeOfDayButton->setStyleSheet("background-color: darkorange; color: black; font: bold 14pt;");
}

timedSettingsProfiles::~timedSettingsProfiles()
{
    delete ui;
}

void timedSettingsProfiles::refreshTimedSettingsList()
{
    ui->tSListWidget->clear();

    QJsonArray timeProfiles = loadTimedSettings();

    for (const auto &val : timeProfiles) {
        QJsonObject obj = val.toObject();

        QString time     = obj["profileTime"].toString();
        double  br       = obj["basalRate"].toDouble();
        int     cF       = obj["correctionFactor"].toInt();
        int     cR       = obj["carbRatio"].toInt();
        int     tBG      = obj["targetBG"].toInt();

        QString topLine = QString("%1%2%3%4%5")
            .arg(QString("%1 u/hr").arg(br, 0, 'f', 1).leftJustified(20))
            .arg(QString("1u:%1 mmol/L").arg(cF).leftJustified(20))
            .arg(QString(" "), 10)
            .arg(QString("1u:%1 g").arg(cR).leftJustified(20))
            .arg(QString("%1 mmol/L").arg(tBG).leftJustified(17));

        QString labelLine = QString("%1%2%3%4%5")
            .arg("   BASAL", -17)
            .arg("   CORR FACTOR", -19)
            .arg("", -5)
            .arg("   CARB RATIO", -15)
            .arg("TARGET BG", 15);

        QString displayText = QString("%1\n%2\n%3").arg(time).arg(topLine).arg(labelLine);

        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, obj);
        ui->tSListWidget->addItem(item);
    }
}

QString timedSettingsProfiles::getTimedSettingsFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/timedSettings.json";
}

int timedSettingsProfiles::findTimedSettingIndex(const QString &name, const QJsonArray &array)
{
    for (int i = 0; i < array.size(); ++i) {
        if (array[i].toObject()["name"].toString() == name)
            return i;
    }
    return -1;
}

void timedSettingsProfiles::on_backButton_TSMenu_clicked()
{
    emit returnToProfilesPage();
    this->close();
}

void timedSettingsProfiles::on_tandemLogoButton_clicked()
{
    emit returnToMainPage();
    this->hide();
}

void timedSettingsProfiles::on_addTSButton_clicked()
{
    if (newTimeProfile) delete newTimeProfile;
    newTimeProfile = new timeProfile();

    QJsonArray profiles = loadTimedSettings();

    if (!profiles.isEmpty()) {
        QJsonObject last = profiles.last().toObject();

        newTimeProfile->basalRate = last["basalRate"].toDouble();
        newTimeProfile->correctionFactor = last["correctionFactor"].toInt();
        newTimeProfile->carbRatio = last["carbRatio"].toInt();
        newTimeProfile->targetBG = last["targetBG"].toInt();
    }

    ui->stackedWidget->setCurrentIndex(1);
}

void timedSettingsProfiles::on_backButton_TimeSegment_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void timedSettingsProfiles::on_startTimeButton_clicked()
{
    bool ok = false;
    int hours = QInputDialog::getInt(
        this,
        "Start Time",
        "Hour:",
        12,
        1,
        12,
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

    QString ampm = ui->timeOfDayButton->text();

    if (ampm == "PM" && hours != 12) {
        hours += 12;
    } else if (ampm == "AM" && hours == 12) {
        hours = 00;
    }

    QTime startTime(hours, minutes);
    QString timeString = startTime.toString("HH:mm");
    newTimeProfile->profileTime = startTime;
    ui->startTimeButton->setText(QString("%1h %2m").arg(hours).arg(minutes));
}

void timedSettingsProfiles::on_timeOfDayButton_clicked()
{
    QString current = ui->timeOfDayButton->text();

    if (current == "AM") {
        ui->timeOfDayButton->setText("PM");
        ui->timeOfDayButton->setStyleSheet("background-color: darkblue; color: white; font: bold 14pt;");
    } else {
        ui->timeOfDayButton->setText("AM");
        ui->timeOfDayButton->setStyleSheet("background-color: darkorange; color: black; font: bold 14pt;");
    }
}


void timedSettingsProfiles::on_checkButton_TimeSegment_clicked()
{
    QJsonObject timeSegment;
    timeSegment["profileTime"] = newTimeProfile->profileTime.toString("HH:mm");
    timeSegment["basalRate"] = newTimeProfile->basalRate;
    timeSegment["correctionFactor"] = newTimeProfile->correctionFactor;
    timeSegment["carbRatio"] = newTimeProfile->carbRatio;
    timeSegment["targetBG"] = newTimeProfile->targetBG;

    QJsonArray segments = loadTimedSettings();
    segments.append(timeSegment);
    saveTimedSettings(segments);

    emit returnToTimedSettingsPage(timeSegment);
    this->hide();
}

void timedSettingsProfiles::handleNewSegmentFromProfile(QJsonObject segment)
{
    QString profileTime = segment["profileTime"].toString();
    QJsonArray timeSegments = loadTimedSettings();

    bool replaced = false;
    for (int i = 0; i < timeSegments.size(); ++i) {
        if (timeSegments[i].toObject()["profileTime"].toString() == profileTime) {
            timeSegments[i] = segment;
            replaced = true;
            break;
        }
    }

    if (!replaced)
        timeSegments.append(segment);

    saveTimedSettings(timeSegments);

    refreshTimedSettingsList();
    ui->stackedWidget->setCurrentIndex(0);
    this->show();
}


void timedSettingsProfiles::on_tSListWidget_itemClicked(QListWidgetItem *item)
{
    if (!item) return;

    selectedTimeSegment = item;
    ui->selectTSButton->setEnabled(true);
    ui->deleteTSButton->setEnabled(true);

}

void timedSettingsProfiles::on_selectTSButton_clicked()
{
    if (!selectedTimeSegment) {
        QMessageBox::warning(this, "No Selection", "Please select a time segment to use.");
        return;
    }

    QJsonObject segment = selectedTimeSegment->data(Qt::UserRole).toJsonObject();

    emit returnToTimedSettingsPage(segment);
    this->hide();
}


void timedSettingsProfiles::on_deleteTSButton_clicked()
{
    if (!selectedTimeSegment) {
        QMessageBox::warning(this, "No Selection", "Please select a time segment to delete.");
        return;
    }

    QJsonObject selectedObj = selectedTimeSegment->data(Qt::UserRole).toJsonObject();
    QString selectedTime = selectedTimeSegment->data(Qt::UserRole).toJsonObject()["profileTime"].toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        "Delete Profile: " + selectedTime + "?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes)
        return;

    QJsonArray segments = loadTimedSettings();
    bool found = false;
    for (int i = 0; i < segments.size(); ++i) {
        if (segments[i].toObject()["profileTime"].toString() == selectedTime) {
            segments.removeAt(i);
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Error", "Time segment not found.");
        return;
    }

    if (!saveTimedSettings(segments)) {
        QMessageBox::warning(this, "Error", "Failed to save updated time segments.");
        return;
    }

    QMessageBox::information(this, "Deleted", "Time segment deleted successfully.");

    selectedTimeSegment = nullptr;
    ui->deleteTSButton->setEnabled(false);
    refreshTimedSettingsList();
}



