#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsmenu.h"
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include "batterymanager.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Start battery draining
    turnOn();

    // Main battery function
    BatteryManager::instance()->updateLabel(ui->batteryLabel);

    // Handle battery depletion (e.g., turn off UI)
    connect(BatteryManager::instance(), &BatteryManager::batteryDepleted, this, &MainWindow::turnOff);

    // Clock
    connect(Config::instance(), &Config::clockUpdated, this, [=](const QString &time){
        ui->timeDateLabel->setText(time);
    });

    // Charge button
    connect(ui->ChargeButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);

    // Unplug button
    connect(ui->UnplugButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);

    // Bolus button (placeholder)
    connect(ui->bolusButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Bolus", "Bolus button clicked. Future feature");
    });

    // Placeholders
    ui->glucoseLabel->setText("5.0 mmol/L");
    ui->iobLabel->setText("IOB: 1.2U");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete config;
}

// Plugging/Unplugging

void MainWindow::turnOff() {
    isOn = false;
    BatteryManager::instance()->stopAll();
}

void MainWindow::turnOn() {
    isOn = true;
    BatteryManager::instance()->startDraining();
}

// Open Options Menu
void MainWindow::on_optionsButton_clicked()
{
    optionsMenu *options = new optionsMenu(this);
    options->show();
}
