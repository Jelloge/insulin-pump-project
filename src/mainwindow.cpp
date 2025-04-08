#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bolusmenu.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

// HOME PAGE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    options = new optionsMenu(this);
    options->hide();

    bolusPage = new bolusmenu(this);
    bolusPage->hide();

    //Initialize Glucose Monitoring
    glucoseMonitoring = new GlucoseMonitoring(this, ui->cgmGraphView, ui->glucoseLabel, ui->timeRangeButton);
    connect(ui->timeRangeButton, &QPushButton::clicked, glucoseMonitoring, &GlucoseMonitoring::cycleTimeRange);
    glucoseMonitoring->start();

    connect(options, &optionsMenu::returnToMainWindow, this, &MainWindow::show);

    // Return to home when home button is clicked
    connect(bolusPage, &bolusmenu::returnToMainWindow, this, &MainWindow::show);


    // Set the battery label
    BatteryManager::instance()->updateLabel(ui->batteryLabel);
    BatteryManager::instance()->plugIn();
    BatteryManager::instance()->unplug();
    BatteryManager::instance()->turnOff();

    // Clock update hookup
    connect(Config::instance(), &Config::clockUpdated, this, [=](const QString &time){
        ui->timeDateLabel->setText(time);
    });

    // Power / Battery buttons
    connect(ui->ChargeButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
    connect(ui->turnOffButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOff);
    connect(ui->turnOnButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOn);

    ui->glucoseLabel->setText("5.0 mmol/L");
    ui->iobLabel->setText("IOB: 1.2U");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete config;
}

// Open Options Menu
void MainWindow::on_optionsButton_clicked()
{
    options->show();
}

// Open Bolus Menu
void MainWindow::on_bolusButton_clicked()
{
    bolusPage->show();
}

void MainWindow::refreshBatteryBindings() {
    BatteryManager::instance()->updateLabel(ui->batteryLabel);
}

