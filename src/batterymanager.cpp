#include "batterymanager.h"
#include <QMetaObject>
#include <QTimer>
#include <QDebug>

BatteryManager::BatteryManager(QObject *parent, QLabel *label, QLabel *icon)
    : QObject(parent), batteryLevel(100), isCharging(false), isOn(false), batteryLabel(label), chargingIcon(icon)
{
    batteryLabel->setText("Battery: 100%");

    batteryDrainTimer = new QTimer(this);
    connect(batteryDrainTimer, &QTimer::timeout, this, &BatteryManager::drainBattery);

    batteryChargeTimer = new QTimer(this);
    connect(batteryChargeTimer, &QTimer::timeout, this, &BatteryManager::chargeBatteryStep);

    if (chargingIcon)
        chargingIcon->clear();
}

void BatteryManager::start() {
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000);
}

void BatteryManager::stop()
{
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
}

void BatteryManager::startDraining() {
    isOn = true;
    batteryDrainTimer->start(500); // drain every 3s
}

void BatteryManager::stopAll() {
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
}


void BatteryManager::plugIn() {
    isCharging = true;
    if (chargingIcon)
        chargingIcon->setPixmap(QPixmap("ui_icons/charging.jpg"));

    batteryChargeTimer->start(500);
}

void BatteryManager::unplug() {
    isCharging = false;
    if (chargingIcon)
        chargingIcon->clear();

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
    batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");
}
