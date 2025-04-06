#include "batterymanager.h"
#include <QMetaObject>
#include <QTimer>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

// Singleton
BatteryManager* BatteryManager::instance() {
    static BatteryManager* _instance = new BatteryManager();
    return _instance;
}

// Constructor
BatteryManager::BatteryManager(QObject *parent)
    : QObject(parent),
      batteryLevel(100),
      isCharging(false),
      isOn(false),
      batteryLabel(nullptr),
      chargingIcon(nullptr)
{
    batteryDrainTimer = new QTimer(this);
    connect(batteryDrainTimer, &QTimer::timeout, this, &BatteryManager::drainBattery);

    batteryChargeTimer = new QTimer(this);
    connect(batteryChargeTimer, &QTimer::timeout, this, &BatteryManager::chargeBatteryStep);
}

// UI Updater
void BatteryManager::updateLabel(QLabel *newBatteryLabel, QLabel *newChargingIcon) {
    batteryLabel = newBatteryLabel;
    chargingIcon = newChargingIcon;

    if (batteryLabel)
        batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");
    if (chargingIcon)
        chargingIcon->clear();
}

// Control Methods
void BatteryManager::start() {
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000);
}

void BatteryManager::startDraining() {
    isOn = true;
    if (!batteryDrainTimer->isActive())
        batteryDrainTimer->start(3000);
}

void BatteryManager::stop() {
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

void BatteryManager::stopAll() {
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

// ---- Plug/Unplug ---- //
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

// Charging and Draining Logic
void BatteryManager::drainBattery() {
    if (!isCharging && isOn && batteryLevel > 0) {
        batteryLevel--;
        updateUI();

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
        batteryChargeTimer->stop();
    }
}

// ---- UI Display ---- //
void BatteryManager::updateUI() {
    if (!batteryLabel) return;

    batteryLabel->setText("Battery: " + QString::number(batteryLevel) + "%");

    if (batteryLevel <= 10) {
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: red;");
        QMessageBox::warning(nullptr, "Battery Low", "Battery is low. Please plug in the charger!");
    } else {
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: lightgreen;");
    }
}
