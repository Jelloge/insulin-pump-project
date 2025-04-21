#include "mainwindow.h"
#include "ui_mainwindow.h"

// HOME PAGE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize Options Menu
    options = new optionsMenu(this);
    options->hide();
    connect(options, &optionsMenu::controlIQStatusUpdated, this, &MainWindow::updateControlIQStatusIcon);
    connect(options, &optionsMenu::basalDeliveryStatusUpdated, this, &MainWindow::updateBasalDeliveryStatusIcon);

    GlucoseMonitoring* glucoseMonitorin = new GlucoseMonitoring(
        this,            // parent QObject
        ui->cgmGraphView,   // your graph view widget
        ui->glucoseLabel, // your glucose label
        ui->timeRangeButton // your time range button
    );

    // Create bolus menu and pass the GlucoseMonitoring instance to it
    bolusPage = new bolusmenu(glucoseMonitorin, this);
    bolusPage->hide();

    //Initialize Glucose Monitoring
    glucoseMonitoring = new GlucoseMonitoring(this, ui->cgmGraphView, ui->glucoseLabel, ui->timeRangeButton);
    connect(ui->timeRangeButton, &QPushButton::clicked, glucoseMonitoring, &GlucoseMonitoring::cycleTimeRange);
    glucoseMonitoring->start();

    // Return to home when home button is clicked
    connect(options, &optionsMenu::returnToMainWindow, this, &MainWindow::show);
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

    // Control-IQ Icon
    QPixmap controlIQONPixmap(":/ui_icons/graydiamond.png");
    QIcon controlIQONIcon(controlIQONPixmap);

    ui->controlIQStatusIcon->setIcon(controlIQONIcon);
    ui->controlIQStatusIcon->setIconSize(QSize(20, 20));
    ui->controlIQStatusIcon->setVisible(false);

    // Basal Delivery Icon
    QPixmap basalDeliveryMaintainPixmap(":/ui_icons/letter-b.png");
    QIcon basalDeliveryIcon(basalDeliveryMaintainPixmap);

    ui->basalDeliveryStatusIcon->setIcon(basalDeliveryIcon);
    ui->basalDeliveryStatusIcon->setIconSize(QSize(20, 20));
    ui->basalDeliveryStatusIcon->setVisible(false);
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

void MainWindow::updateControlIQStatusIcon(bool visible) {
    ui->controlIQStatusIcon->setVisible(visible);
}

void MainWindow::updateBasalDeliveryStatusIcon(bool visible) {
    ui->basalDeliveryStatusIcon->setVisible(visible);
}
