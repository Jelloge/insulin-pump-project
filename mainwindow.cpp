#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

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
// COMMENTED OUT: Methods
// until we actually implement them or hook them to the new UI
// ------------------------------------------

void MainWindow::turnOff() {
    isOn = false;
    batteryManager->stopAll();
}

void MainWindow::turnOn() {
    isOn = true;
    batteryManager->startDraining();
}

bool MainWindow::checkingPIN() {
    // Future: if you have a PIN system
    return false;
}

void MainWindow::returnHome() {
    // Future: if you reintroduce multi-page logic
}

void MainWindow::chargerUnplugged() {
    batteryManager->unplug();
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
