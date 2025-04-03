#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Connect the calculate button to calculateBolus slot
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculateBolus);
    connect(ui->cancelBolusButton, &QPushButton::clicked, this, &MainWindow::cancelBolus);

    connect(ui->overrideButton, &QPushButton::clicked, this, &MainWindow::calculateBolus);


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
    double currentBG = 7;
    double iob = 5;

    // Calculate total bolus based on carbs, BG, and IOB
    double bolus = bolusCalc.calculateTotalBolus(carbs, currentBG, iob);

    // Check if the override button was pressed
    if (sender() == ui->overrideButton) {
        QString overrideText = ui->overrideBolusInput->text();
        bool ok;
        double overrideBolus = overrideText.toDouble(&ok);

        if (ok && overrideBolus > 0) {  // Ensure valid positive number
            bolus = overrideBolus;
        }
    }

    FinalBolus = bolus;
    bolusCalc.splitBolus(bolus, immediateBolus, extendedBolus);
    isBolusActive = true; // Mark bolus as active

    updateBolusDisplay(); // Update UI based on checkboxes




    // Reset totalCarbs after calculation
    totalCarbs = 0;
    ui->stackedWidget->setCurrentIndex(1); // Switch to result page
}


void MainWindow::updateBolusDisplay() {
    QString result;

    double hours = ui->hourInput->text().toDouble();
    double bolusPerHour = (hours > 0) ? (extendedBolus / hours) : 0; // Avoid division by zero

    if (ui->showBothCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units\nExtended Bolus: %3 units\nBolus Per Hour: %4 units")
                     .arg(FinalBolus)
                     .arg(immediateBolus)
                     .arg(extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));

    }
    else if (ui->showImmediateCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nImmediate Bolus: %2 units")
                     .arg(FinalBolus)
                     .arg(immediateBolus);
    }
    else if (ui->showExtendedCheckBox->isChecked()) {
        result = QString("Final Bolus: %1 units\nExtended Bolus: %2 units\nBolus Per Hour: %3 units")
                     .arg(FinalBolus)
                     .arg(extendedBolus)
                     .arg(QString::number(bolusPerHour, 'f', 2));
    }

    ui->resultTextEdit->setPlainText(result);  // Display the result in the QTextEdit
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





