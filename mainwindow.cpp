#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
// #include <QtConcurrent/QtConcurrent>  // concurrency for battery logic

// If we have a configData or other classes, include them here
// #include "configdata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the OPTIONS button
    connect(ui->optionsButton, &QPushButton::clicked, [=]() {
        // Right now, no second page for Options, so just show a message
        QMessageBox::information(this, "Options", "Options button clicked. Future feature");
    });


    // connect the charge button

    connect(ui->ChargeButton, &QPushButton::clicked, this, &MainWindow::chargeDevice);

    // connect unplug button

    connect(ui->UnplugButton, &QPushButton::clicked, this, &MainWindow::unplugCharger);

    // connect the BOLUS button
    connect(ui->bolusButton, &QPushButton::clicked, [=]() {
        // Show a message or transition to a Bolus feature when ready
        QMessageBox::information(this, "Bolus", "Bolus button clicked. Future feature");
    });

    // The Tandem logo button (home):
    connect(ui->tandemLogoButton, &QPushButton::clicked, [=]() {
        // If we only have one page (home), we can just pop a message
        QMessageBox::information(this, "Home", "Already at the Home screen!");
    });

    //
    // show the time updating every second, intervals of one second
    //
    QTimer *clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, [=]() {
        ui->timeDateLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss  |  dd MMM"));
    });
    clockTimer->start(1000);

    //
    // Set initial battery + glucose + IOB values
    //
    // JUST PLACEHOLDERS FOR NOW
    ui->batteryLabel->setText("Battery: 100%");
    ui->glucoseLabel->setText("5.0 mmol/L");
    ui->iobLabel->setText("IOB: 1.2U");

    //
    //concurrency for battery draining or config classes, add them here
    //
    // batteryLevel = 100;
    // isCharging = false;
    // configData = new ConfigData(...);
}

MainWindow::~MainWindow()
{
    delete ui;
    // If you have configData or other allocated objects, delete them here
    // delete configData;
}

// ------------------------------------------
// COMMENTED OUT: Old Slots & Methods
// until we actually implement them or hook them to the new UI
// ------------------------------------------

void MainWindow::turnOff() {
    // Future: if you want an OFF screen
}

void MainWindow::turnOn() {
    isOn = true;
    startBatteryDrain();
}


bool MainWindow::checkingPIN() {
    // Future: if you have a PIN system
    return false;
}

void MainWindow::returnHome() {
    // Future: if you reintroduce multi-page logic
}

void MainWindow::chargeBattery() {
    if (isCharging || batteryLevel >= 100) return;

    isCharging = true;

    batteryChargeTimer = new QTimer(this);
    connect(batteryChargeTimer, &QTimer::timeout, this, [=]() {
        if (batteryLevel < 100) {
            batteryLevel++;
            ui->BatteryBar->setValue(batteryLevel);
            ui->batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");
        } else {
            batteryChargeTimer->stop();
        }
    });

    batteryChargeTimer->start(500); // charge every 0.5s
}

void MainWindow::chargerUnplugged() {
    isCharging = false;
    if (batteryChargeTimer) batteryChargeTimer->stop();
    // Future: for battery logic
}

void MainWindow::BatteryDrain() {
    if (!isOn) return;

    batteryDrainTimer = new QTimer(this);
    connect(batteryDrainTimer, &QTimer::timeout, this, [=]() {
        if (!isCharging && batteryLevel > 0) {
            batteryLevel--;
            ui->BatteryBar->setValue(batteryLevel);
            ui->batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");

            if (batteryLevel <= 10) {
                ui->batteryLabel->setStyleSheet("color: red;");
            }

            if (batteryLevel <= 0) {
                batteryDrainTimer->stop();
                turnOff(); // simulate device turning off
            }
        }
    });

    batteryDrainTimer->start(3000); // drains 1% every 3 sec
}
void MainWindow::CreateProfileClicked(){
    // Future: if you add profiles
}

void MainWindow::AddCarbsClicked(){
    // Future: your user enters carbs
}

void MainWindow::AddBGClicked(){
    // Future: user adds blood glucose
}

void MainWindow::ConfirmBolusClicked(){
    // Future: confirm a bolus
}

void MainWindow::CancelBolusEntry(){
    // ...
}

void MainWindow::ConfirmBolusRejected(){
    // ...
}

void MainWindow::SetDeliverySplitClicked(){
    // ...
}

void MainWindow::SetDurationClicked(){
    // ...
}

void MainWindow::SetDeliveryTimeClicked(){
    // ...
}
