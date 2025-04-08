#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Connect the calculate button to calculateBolus slot
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculateBolus);
    connect(ui->cancelBolusButton, &QPushButton::clicked, this, &MainWindow::cancelBolus);

    connect(ui->overrideButton, &QPushButton::clicked, this, &MainWindow::calculateBolus);
    connect(ui->deleteHistoryButton, &QPushButton::clicked, this, &MainWindow::deleteHistory);

    // Connect the "Add Carbs" button to the addCarbs function
    connect(ui->addCarbButton, &QPushButton::clicked, this, &MainWindow::addCarbs);
    connect(ui->goBackButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);  // Switch back to the input page (index 0)
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::calculateBolus() {
    double carbs = totalCarbs;  // Use the accumulated totalCarbs value
    double currentBG = ui->currentBGInput->text().toDouble();
    double iob = 5;

    // Calculate total bolus based on carbs, BG, and IOB
    double bolus = bolusCalc.calculateTotalBolus(carbs, currentBG, iob);
    QString overrideMsg = "";

    // Check if the override button was pressed
    if (sender() == ui->overrideButton) {
        QString overrideText = ui->overrideBolusInput->text();
        bool ok;
        double overrideBolus = overrideText.toDouble(&ok);

        if (ok && overrideBolus > 0) {  // Ensure valid positive number
            bolus = overrideBolus;
            overrideMsg = "Override applied: Custom bolus value";
        }
    }

    FinalBolus = bolus;
    bolusCalc.splitBolus(bolus, immediateBolus, extendedBolus);
    isBolusActive = true; // Mark bolus as active
    double duration = 3;
    currentBolusInfo = BolusInfo(FinalBolus, immediateBolus, extendedBolus, duration, overrideMsg);  // Pass the override message

    updateBolusDisplay(); // Update UI based on checkboxes


    logBolus(currentBolusInfo);


    // Reset totalCarbs after calculation
    totalCarbs = 0;
    ui->stackedWidget->setCurrentIndex(1); // Switch to result page
    ui->currentBGInput->clear();
    ui->carbInput->clear();
    ui->overrideBolusInput->clear();
}


void MainWindow::updateBolusDisplay() {
    QString result;
    double bolusPerHour = (currentBolusInfo.durationHours > 0)
                          ? (currentBolusInfo.extendedBolus / currentBolusInfo.durationHours)
                          : 0;

    if (ui->showBothCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units\nExtended Bolus: %3 units\nBolus Per Hour: %4 units")
                     .arg(currentBolusInfo.finalBolus)
                     .arg(currentBolusInfo.immediateBolus)
                     .arg(currentBolusInfo.extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));
    }
    else if (ui->showImmediateCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units")
                     .arg(currentBolusInfo.finalBolus)
                     .arg(currentBolusInfo.immediateBolus);
    }
    else if (ui->showExtendedCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nExtended Bolus: %2 units\nBolus Per Hour: %3 units")
                     .arg(currentBolusInfo.finalBolus)
                     .arg(currentBolusInfo.extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));
    }
    else {
        result = QString("Final Bolus: %1 units").arg(currentBolusInfo.finalBolus);
    }

    ui->resultTextEdit->setPlainText(result);
}



void MainWindow::addCarbs() {
    double inputCarbs = ui->carbInput->text().toDouble();  // Get the value from the QLineEdit
    totalCarbs += inputCarbs;  // Add to the accumulated total

    // Update the QLineEdit to display the accumulated carbs
    ui->carbInput->setText(QString::number(totalCarbs, 'f', 1));  // Show 1 decimal point for clarity
}
void MainWindow::cancelBolus() {
    if (isBolusActive) {
        // Stop the extended bolus by resetting values
        extendedBolus = 0;
        isBolusActive = false;

        // Update display to reflect cancellation
        ui->resultTextEdit->setPlainText("Bolus delivery canceled.");
    } else {
        QMessageBox::information(this, "Info", "No active bolus to cancel.");
    }
    ui->stackedWidget->setCurrentIndex(1); // Since ResultPage is at index 1

}
void MainWindow::logBolus(const BolusInfo& info) {
    // Specify the full path to save the file
    QString filename = "/home/student/ESample/bolus_log.json";
    QFile file(filename);

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

void MainWindow::deleteHistory() {
    QString filename = "/home/student/ESample/bolus_log.json";  // Path to your log file
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








