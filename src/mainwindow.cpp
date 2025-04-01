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

    glucoseMonitoring = new GlucoseMonitoring(this);
    connect(glucoseMonitoring, &GlucoseMonitoring::newReading, this, &MainWindow::updateGraph);
    connect(glucoseMonitoring, &GlucoseMonitoring::lowGlucoseAlert, this, &MainWindow::onLowGlucoseAlert);

    setupGraph();

    glucoseMonitoring->start(); // Start glucose monitoring

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
    ui->glucoseLabel->setText("5.0 mmol/L");
    ui->iobLabel->setText("IOB: 1.2U");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete batteryManager;
    delete config;
    delete glucoseMonitoring;
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

// ---------- GLUCOSE MONITORING ----------

void MainWindow::setupGraph() {
    cgmGraphView = new QChart();
    series = new QLineSeries();
    cgmGraphView->addSeries(series);
    cgmGraphView->legend()->hide();
    cgmGraphView->setBackgroundBrush(QBrush(QColor("black")));

    // Setup X-axis (Time)
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("hh:mm");
    axisX->setTickCount(7); // Number of time labels
    axisX->setGridLineVisible(false);
    cgmGraphView->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Setup Y-axis (Glucose mmol/L)
    QValueAxis *axisY = new QValueAxis;
    axisY->setGridLineVisible(false);
    axisY->setRange(0.0, 25.0);
    cgmGraphView->addAxis(axisY, Qt::AlignRight);
    series->attachAxis(axisY);

    // Initialize member variables
    lowerLine = new QLineSeries();
    upperLine = new QLineSeries();
    targetArea = new QAreaSeries(upperLine, lowerLine);

    // Add points to cover the entire x-range
    lowerLine->append(0, 2.2);
    lowerLine->append(1, 2.2);
    upperLine->append(0, 10);
    upperLine->append(1, 10);

    // Create the area series between lower and upper lines
    //QAreaSeries *targetArea = new QAreaSeries(upperLine, lowerLine);
    targetArea->setColor(QColor(100, 100, 100, 100));
    targetArea->setBorderColor(Qt::transparent);
    cgmGraphView->addSeries(targetArea);
    targetArea->attachAxis(axisX);
    targetArea->attachAxis(axisY);

    // RED LINE at 2.2 mmol/L
    lowerLine->setColor(Qt::red);
    cgmGraphView->addSeries(lowerLine);
    lowerLine->attachAxis(axisX);
    lowerLine->attachAxis(axisY);

    // ORANGE LINE at 10 mmol/L
    upperLine->setColor(Qt::darkYellow);
    cgmGraphView->addSeries(upperLine);
    upperLine->attachAxis(axisX);
    upperLine->attachAxis(axisY);

    // Apply to UI
    ui->cgmGraphView->setChart(cgmGraphView);
    ui->cgmGraphView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::updateGraph(double value) {
    QDateTime now = QDateTime::currentDateTime();
    series->append(now.toMSecsSinceEpoch(), value);

    // Get the time 3 hours ago
    QDateTime threeHoursAgo = now.addSecs(-3 * 3600);
    qint64 minTime = threeHoursAgo.toMSecsSinceEpoch();
    qint64 maxTime = now.toMSecsSinceEpoch();

    // Update the boundary lines and area
    lowerLine->clear();
    lowerLine->append(minTime, 2.2);
    lowerLine->append(maxTime, 2.2);

    upperLine->clear();
    upperLine->append(minTime, 10);
    upperLine->append(maxTime, 10);

    // Ensure the X-axis shows only the last 3 hours
    QList<QAbstractAxis *> axes = cgmGraphView->axes(Qt::Horizontal);
    if (!axes.isEmpty()) {
        QDateTimeAxis *axisX = qobject_cast<QDateTimeAxis *>(axes.first());
        if (axisX) {
            axisX->setRange(threeHoursAgo, now);
        }
    }

    // Check for LOW and HIGH glucose levels
    if (value <= 4.0) {
        ui->glucoseLabel->setText(QString("LOW (%1 mmol/L)").arg(value));
        ui->glucoseLabel->setStyleSheet("color: red; font-weight: bold;");
    } else if (value >= 22.2) {
        ui->glucoseLabel->setText(QString("HIGH (%1 mmol/L)").arg(value));
        ui->glucoseLabel->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        ui->glucoseLabel->setText(QString("%1 mmol/L").arg(value));
        ui->glucoseLabel->setStyleSheet("");  // Reset style if normal
    }

    cgmGraphView->update();
}

void MainWindow::onLowGlucoseAlert(double value) {
    QMessageBox::warning(this, "Low Glucose Alert",
                         QString("Glucose dropped to %1 mmol/L! Take action.").arg(value));
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
