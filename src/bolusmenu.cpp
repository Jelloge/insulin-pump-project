#include "bolusmenu.h"
#include "ui_bolusmenu.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QStandardPaths>

bolusmenu::bolusmenu(GlucoseMonitoring* gm, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::bolusmenu)
    , glucoseMonitoring(gm) // store the pointer
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Autofill BG input with latest reading from CGM
    if (glucoseMonitoring) {
        //double latestReading = glucoseMonitoring->getLatestGlucoseReading();
        double latestReading = glucoseMonitoring->getLatestReading();
        ui->currentBGInput->setText(QString::number(latestReading));
    }

    // Button connections
    connect(ui->calculateButton, &QPushButton::clicked, this, &bolusmenu::calculateBolus);
    connect(ui->cancelBolusButton, &QPushButton::clicked, this, &bolusmenu::cancelBolus);

    connect(ui->overrideButton, &QPushButton::clicked, this, &bolusmenu::applyOverride);
    connect(ui->deleteHistoryButton, &QPushButton::clicked, this, &bolusmenu::deleteHistory);

    connect(ui->addCarbButton, &QPushButton::clicked, this, &bolusmenu::addCarbs);
    connect(ui->goBackButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
}


bolusmenu::~bolusmenu() {
    delete ui;
}

void bolusmenu::lloadActiveProfile() {
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        return;
    }

    QJsonArray profiles = doc.array();
    for (const QJsonValue& val : profiles) {
        QJsonObject obj = val.toObject();
        if (obj.contains("isActive") && obj["isActive"].toBool()) {
            if (obj.contains("basalRate"))
                profileBasalRate = obj["basalRate"].toDouble();

            if (obj.contains("carbRatio"))
                profileCarbRatio = obj["carbRatio"].toDouble();

            if (obj.contains("carbsEnabled"))
                profileCarbsEnabled = obj["carbsEnabled"].toBool();

            if (obj.contains("correctionFactor")) {
                profileCorrectionFactor = obj["correctionFactor"].toDouble();
            }

            if (obj.contains("insulinDuration"))
                profileInsulinDuration = obj["insulinDuration"].toInt();

            if (obj.contains("maxBolus"))
                profileMaxBolus = obj["maxBolus"].toDouble();

            if (obj.contains("name"))
                profileName = obj["name"].toString();

            if (obj.contains("targetBG")) {
                profileTargetBG = obj["targetBG"].toDouble();

            }
        }
    }
}


void bolusmenu::calculateBolus() {
    lloadActiveProfile();
    double carbs = totalCarbs;
    double iob = 5;
    Almost = 1;
    confirmBGInput();

    double bolus = bolusCalc.calculateTotalBolus(carbs, iob) + Almost;
    // Save for override
    lastCarbs = carbs;
    lastIOB = iob;
    lastBG = ui->currentBGInput->text().toDouble();
    lastAlmost = Almost;
    lastTotalBolus = bolus;

    FinalBolus = bolus;
    if (!checkMaxBolus(FinalBolus)) {
         return;  // User chose to cancel
     }
    // ✅ Check if extended bolus is enabled
    if (ui->showExtendedCheckBox->isChecked()) {
        double deliverNowPercent = ui->DeliverNow->text().toDouble();
        double deliverLaterPercent = ui->DeliverLater->text().toDouble();

        if (deliverNowPercent + deliverLaterPercent != 100) {
            QMessageBox::warning(this, "Invalid Input", "Deliver Now and Deliver Later percentages must add up to 100.");
            return;
        }

        // Save for override
        lastDeliverNowPercent = deliverNowPercent;


        // Calculate split
        bolusCalc.splitBolus(FinalBolus, deliverNowPercent, immediateBolus, extendedBolus);
    } else {
        // No extended bolus: everything is immediate
        immediateBolus = FinalBolus;
        extendedBolus = 0;
    }

    MidDilveryCancel = true;

    double durationHours = profileInsulinDuration / 60;
    currentBolusInfo = BolusInfo(FinalBolus, immediateBolus, extendedBolus, durationHours, "");

    updateBolusDisplay();
    logBolus(currentBolusInfo);

    // Clear inputs — backup values are preserved
    totalCarbs = 0;
    ui->stackedWidget->setCurrentIndex(1);
    ui->currentBGInput->clear();
    ui->carbInput->clear();
    ui->overrideBolusInput->clear();
}

void bolusmenu::applyOverride() {
     lloadActiveProfile();
    QString overrideText = ui->overrideBolusInput->text();
    bool ok;
    double overrideBolus = overrideText.toDouble(&ok);

    if (ok && overrideBolus > 0) {
        QString overrideMsg = "Override applied: Custom bolus value";

        FinalBolus = overrideBolus;
        if (!checkMaxBolus(FinalBolus)) {
            return;  // User chose to cancel
        }

        // ✅ Check if extended bolus is enabled
        if (ui->showExtendedCheckBox->isChecked()) {
            // Reuse lastDeliverNowPercent to calculate split
            bolusCalc.splitBolus(FinalBolus, lastDeliverNowPercent, immediateBolus, extendedBolus);


        } else {
            // No extended bolus: everything is immediate
            immediateBolus = FinalBolus;
            extendedBolus = 0;
        }

        MidDilveryCancel = true;
        double durationHours = profileInsulinDuration / 60;  // Convert seconds to hours

        currentBolusInfo = BolusInfo(FinalBolus, immediateBolus, extendedBolus, durationHours, "");

        OverrideupdateBolusDisplay();
        logBolus(currentBolusInfo);

        // Clear inputs
        totalCarbs = 0;
        ui->stackedWidget->setCurrentIndex(1);
        ui->currentBGInput->clear();
        ui->carbInput->clear();
        ui->overrideBolusInput->clear();
    } else {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid override bolus value.");
    }
}

bool bolusmenu::checkMaxBolus(double& bolus) {
    if (bolus > profileMaxBolus) {
        QString msg = QString("You requested a bolus of %1 units, which is higher than the Max Bolus setting of %2 units in your active profile.\n\n"
                              "Would you like to proceed with the Max Bolus amount?")
                              .arg(bolus)
                              .arg(profileMaxBolus);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Max Bolus Alert 1",
                                      msg,
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return false;  // User cancelled
        } else {
            bolus = profileMaxBolus;  // Use capped value
        }
    }
    return true;  // Proceed
}


void bolusmenu::confirmBGInput() {
     lloadActiveProfile();
    double currentBG = ui->currentBGInput->text().toDouble();  // Read from UI
    Almost = 1;  // Reset Almost at the beginning of each confirmation
    if (currentBG < profileTargetBG) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Correction Bolus",
            "Your current BG is lower than the target. Would you like to apply a correction bolus?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            double correctionBolus = (profileTargetBG - currentBG) / profileCorrectionFactor;
            Almost = - correctionBolus;  // Subtract correction bolus if BG is low
        }
        else if(reply == QMessageBox::No){ Almost = 0;}
        // If the user presses No, do nothing
    } else if (currentBG > profileTargetBG) {
        double correctionBolus = (currentBG - profileTargetBG) / profileCorrectionFactor;
        Almost = correctionBolus;  // Add correction bolus if BG is high
    }
}

void bolusmenu::updateBolusDisplay() {
    lloadActiveProfile();

    QString result;
    double durationHours = profileInsulinDuration / 60;  // Convert seconds to hours


    if (ui->showExtendedCheckBox->isChecked()) {
        double bolusPerHour = (durationHours > 0)
                              ? (currentBolusInfo.extendedBolus / durationHours)
                              : 0;

        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units\nBolus Per Hour: %3 units")
                     .arg(currentBolusInfo.finalBolus)
                     .arg(currentBolusInfo.extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));
    } else {
        result = QString("Final Bolus: %1 units")
                     .arg(currentBolusInfo.finalBolus);
    }

    ui->resultTextEdit->setPlainText(result);
}
void bolusmenu::OverrideupdateBolusDisplay() {
    lloadActiveProfile();

    QString result;
    double OverridedurationHours = profileInsulinDuration / 60;  // Convert seconds to hours


    if (ui->showExtendedCheckBox->isChecked()) {
        double bolusPerHour = (OverridedurationHours > 0)
                              ? (currentBolusInfo.extendedBolus / OverridedurationHours)
                              : 0;

        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units\nBolus Per Hour: %3 units")
                     .arg(currentBolusInfo.finalBolus)
                     .arg(currentBolusInfo.extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));
    } else {
        result = QString("Final Bolus: %1 units")
                     .arg(currentBolusInfo.finalBolus);
    }

    ui->resultTextEdit->setPlainText(result);
}



void bolusmenu::addCarbs() {
    lloadActiveProfile();
    if (!profileCarbsEnabled) {
        QMessageBox::warning(this, "Carbs Disabled", "Carbs option has not been enabled in your profile. The value is 0.");
        totalCarbs = 0;
      return;
    }

    double inputCarbs = ui->carbInput->text().toDouble();  // Get the value from the QLineEdit
    totalCarbs += inputCarbs;  // Add to the accumulated total
    // Update the QLineEdit to display the accumulated carbs
    ui->carbInput->setText(QString::number(totalCarbs, 'f', 1));  // Show 1 decimal point for clarity
}

void bolusmenu::cancelBolus() {
    if (MidDilveryCancel) {
        // Stop the extended bolus by resetting values
        extendedBolus = 0;
        MidDilveryCancel = false;

        // Update display to reflect cancellation
        ui->resultTextEdit->setPlainText("Bolus delivery canceled.");
    } else {
        QMessageBox::information(this, "Info", "No active bolus to cancel.");
    }
    ui->stackedWidget->setCurrentIndex(1); // Since ResultPage is at index 1

}
void bolusmenu::logBolus(const BolusInfo& info) {
    // Specify the full path to save the file
    QString filename = QCoreApplication::applicationDirPath() + "/../bolus_log.json";

    QFile file(filename);
   // qDebug() << QCoreApplication::applicationDirPath(); // If you wanna find the path to wwhere the json  file is locatted for history.
    QJsonArray logArray;

    // Read existing log data if file exists
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isArray()) {
                logArray = doc.array();
            }
            file.close();
        }
    }

    // Append new entry
    logArray.append(info.toJson());

    // Write updated log
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument newDoc(logArray);
        file.write(newDoc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void bolusmenu::deleteHistory() {
    QString filename = QCoreApplication::applicationDirPath() + "/../bolus_log.json";
    QFile file(filename);

    if (file.exists()) {
        if (file.remove()) {
            QMessageBox::information(this, "Success", "Bolus history deleted successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete bolus history.");
        }
    } else {
        QMessageBox::information(this, "Info", "No history found.");
    }
}

void bolusmenu::on_backButton_clicked() {
    emit returnToMainWindow();
    this->close();
}



