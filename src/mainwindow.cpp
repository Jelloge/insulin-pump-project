#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QDateTimeAxis>
#include  <QValueAxis>
#include <QAreaSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize Battery Manager
    batteryManager = new BatteryManager(this, ui->batteryLabel);

    //Initialize Glucose Monitoring
    glucoseMonitoring = new GlucoseMonitoring(this, ui->cgmGraphView, ui->glucoseLabel, ui->timeRangeButton);
    connect(ui->timeRangeButton, &QPushButton::clicked, glucoseMonitoring, &GlucoseMonitoring::cycleTimeRange);
    glucoseMonitoring->start();

    // 🔥 Call turnOn() to start battery drain
    turnOn();

    // automatically turn off when battery is depleted
    connect(batteryManager, &BatteryManager::batteryDepleted, this, &MainWindow::turnOff);


    // connect the OPTIONS button
    connect(ui->optionsButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Options", "Options button clicked. Future feature");
    });

    // charge button
    connect(ui->ChargeButton, &QPushButton::clicked, batteryManager, &BatteryManager::plugIn);

    // unplug button
    connect(ui->UnplugButton, &QPushButton::clicked, batteryManager, &BatteryManager::unplug);

    // bolus button
    connect(ui->bolusButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Bolus", "Bolus button clicked. Future feature");
    });

    // home (Tandem logo) button
    connect(ui->tandemLogoButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Home", "Already at the Home screen!");
    });

    // clock updater
    QTimer *clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, [=]() {
        ui->timeDateLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss  |  dd MMM"));
    });
    clockTimer->start(1000);

    // placeholder values
    ui->iobLabel->setText("IOB: 1.2U");
}

MainWindow::~MainWindow()
{
    delete glucoseMonitoring;
    delete ui;
    delete batteryManager;
    delete config;

}

// ---------- POWER MANAGEMENT ----------

void MainWindow::turnOff() {
    isOn = false;
    batteryManager->stopAll();
}

void MainWindow::turnOn() {
    isOn = true;
    batteryManager->startDraining();
}

bool MainWindow::checkingPIN() {
    return false; // placeholder
}

void MainWindow::returnHome() {
    // placeholder
}

// ---------- FUTURE FEATURES / STUBS ---------- FEEL FREE TO MODIFY OR ADD YOUR METHODS !

void MainWindow::CreateProfileClicked() {}
void MainWindow::AddCarbsClicked() {}
void MainWindow::AddBGClicked() {}
void MainWindow::ConfirmBolusClicked() {}
void MainWindow::CancelBolusEntry() {}
void MainWindow::ConfirmBolusRejected() {}
void MainWindow::SetDeliverySplitClicked() {}
void MainWindow::SetDurationClicked() {}
void MainWindow::SetDeliveryTimeClicked() {}
