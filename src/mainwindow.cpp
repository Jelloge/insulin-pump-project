#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    options = new optionsMenu(this);
    options->hide();

    connect(options, &optionsMenu::returnToMainWindow, this, &MainWindow::show);connect(options, &optionsMenu::returnToMainWindow, this, &MainWindow::show);

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
    connect(ui->ChargeButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);

    connect(ui->bolusButton, &QPushButton::clicked, this, [=]() {
        QMessageBox::information(this, "Bolus", "Bolus button clicked. Future feature");
    });

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

void MainWindow::refreshBatteryBindings() {
    BatteryManager::instance()->updateLabel(ui->batteryLabel);
}

