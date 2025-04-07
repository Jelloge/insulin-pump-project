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

    // Set the battery label
    BatteryManager::instance()->updateLabel(ui->batteryLabel);
    BatteryManager::instance()->plugIn();
    BatteryManager::instance()->unplug();

    // Clock update hookup
    connect(Config::instance(), &Config::clockUpdated, this, [=](const QString &time){
        ui->timeDateLabel->setText(time);
    });

    connect(ui->ChargeButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
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
    optionsMenu *options = new optionsMenu(this);
    options->show();
}
