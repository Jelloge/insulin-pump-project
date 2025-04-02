#include "batterymanager.h"
#include <QMetaObject>
#include <QTimer>
#include <QDebug>
#include <QPixmap>

BatteryManager::BatteryManager(QObject *parent, QLabel *label, QLabel *icon)
    : QObject(parent), batteryLevel(100), isCharging(false), isOn(false), batteryLabel(label), chargingIcon(icon)
{
    batteryLabel->setText("Battery: 100%");
    if (chargingIcon) chargingIcon->clear();

    batteryDrainTimer = new QTimer(this);
    connect(batteryDrainTimer, &QTimer::timeout, this, &BatteryManager::drainBattery);

    batteryChargeTimer = new QTimer(this);
    connect(batteryChargeTimer, &QTimer::timeout, this, &BatteryManager::chargeBatteryStep);
}

void BatteryManager::start() {
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000); //test
}

void BatteryManager::stopAll() {
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

void BatteryManager::stop()
{
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

void BatteryManager::startDraining() {
    isOn = true;
    if (!batteryDrainTimer->isActive())
        batteryDrainTimer->start(3000);
}

void BatteryManager::plugIn() {
    if (isCharging || batteryLevel >= 100) return;

    isCharging = true;

    if (chargingIcon)
        chargingIcon->setPixmap(QPixmap(":/icons/charging.png").scaled(20, 20, Qt::KeepAspectRatio));

    if (!batteryChargeTimer->isActive())
        batteryChargeTimer->start(1000);
}

void BatteryManager::unplug() {
    isCharging = false;
    if (chargingIcon) chargingIcon->clear();
    batteryChargeTimer->stop();
}

void BatteryManager::drainBattery()
{
    if (!isCharging && isOn && batteryLevel > 0) {
        batteryLevel--;
        updateUI();

        if (batteryLevel <= 10) {
            batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: red;");
        } else {
            batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: lightgreen;");
        }

        if (batteryLevel == 0) {
            qDebug() << "Battery dead, turning off...";
            emit batteryDepleted();
            stop();
        }
    }
}

void BatteryManager::chargeBatteryStep() {
    if (isCharging && batteryLevel < 100) {
        batteryLevel++;
        updateUI();
    } else if (batteryLevel >= 100) {
        batteryChargeTimer->stop();  // Stop charging, remain at 100%
    }
}

void BatteryManager::updateUI()
{
    batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");

    if (batteryLevel <= 10) {
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: red;");
    } else {
        // Restore green + bold styling
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: lightgreen;");  // light green
    }
}

