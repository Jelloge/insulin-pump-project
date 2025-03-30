#include "batterymanager.h"
#include <QMetaObject>
#include <QTimer>
#include <QDebug>

BatteryManager::BatteryManager(QObject *parent, QLabel *label, QProgressBar *bar)
    : QObject(parent), batteryLevel(100), isCharging(false), isOn(false), batteryLabel(label), batteryBar(bar) {
    // Set initial values
    batteryBar->setValue(batteryLevel);
    batteryLabel->setText("Battery: 100%");

    batteryDrainTimer = new QTimer(this);
    connect(batteryDrainTimer, &QTimer::timeout, this, &BatteryManager::drainBattery);

    batteryChargeTimer = new QTimer(this);
    connect(batteryChargeTimer, &QTimer::timeout, this, &BatteryManager::chargeBatteryStep);
}

void BatteryManager::start()
{
    isOn = true;
    batteryDrainTimer->start(3000); // Drain 1% every 3 seconds
}

void BatteryManager::stop()
{
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
}

void BatteryManager::plugIn()
{
    isCharging = true;
    batteryChargeTimer->start(500); // Charge 1% every 0.5 seconds
}

void BatteryManager::unplug()
{
    isCharging = false;
    batteryChargeTimer->stop();
}

void BatteryManager::drainBattery()
{
    if (!isCharging && isOn && batteryLevel > 0) {
        batteryLevel--;
        updateUI();

        if (batteryLevel <= 10) {
            batteryLabel->setStyleSheet("color: red;");
        }

        if (batteryLevel == 0) {
            qDebug() << "Battery dead, turning off...";
            emit batteryDepleted();
            stop();
        }
    }
}

void BatteryManager::chargeBatteryStep()
{
    if (isCharging && batteryLevel < 100) {
        batteryLevel++;
        updateUI();

        if (batteryLevel == 100) {
            batteryChargeTimer->stop();
        }
    }
}

void BatteryManager::updateUI()
{
    batteryBar->setValue(batteryLevel);
    batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");
}
