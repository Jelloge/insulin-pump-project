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

// Called from each page to bind the current UI's battery widgets
void BatteryManager::updateLabel(QLabel *newBatteryLabel, QLabel *newChargingIcon) {
    batteryLabel = newBatteryLabel;
    chargingIcon = newChargingIcon;

    updateUI();

    if (chargingIcon)
        chargingIcon->clear();  // Reset icon
}

// Start full battery monitoring
void BatteryManager::start() {
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000);
}

// Used on each page switch to make sure the draining continues
void BatteryManager::startDraining() {
    isOn = true;

    if (!batteryLabel) {
        qDebug() << "[BatteryManager] No battery label set!";
        return;
    }

    if (!batteryDrainTimer->isActive())
        batteryDrainTimer->start(3000);
}

// Stops both timers and charging icon
void BatteryManager::stop() {
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

// Kill all operations
void BatteryManager::stopAll() {
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

// Starts charging the battery
void BatteryManager::plugIn() {
    if (isCharging || batteryLevel >= 100) return;

    isCharging = true;

    if (chargingIcon)
        chargingIcon->setPixmap(QPixmap(":/ui_icons/charging.jpg").scaled(20, 20, Qt::KeepAspectRatio));

    if (!batteryChargeTimer->isActive())
        batteryChargeTimer->start(1000);
}

// Stops charging
void BatteryManager::unplug() {
    isCharging = false;
    if (chargingIcon) chargingIcon->clear();
    batteryChargeTimer->stop();
}

// Battery goes down 1% every 3s if not charging
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

// Turn on device
void BatteryManager::turnOn() {
    if (isOn) return;
    isOn = true;
    startDraining();
    qDebug() << "Device turned ON";
}

// Turn off device
void BatteryManager::turnOff() {
    if (!isOn) return;
    isOn = false;
    stopAll();
    qDebug() << "Device turned OFF";
}


// Battery goes up 1% every 1s when charging
void BatteryManager::chargeBatteryStep() {
    if (isCharging && batteryLevel < 100) {
        batteryLevel++;
        updateUI();
    } else if (batteryLevel >= 100) {
        batteryChargeTimer->stop();
    }
}

// Updates UI text, color, and style
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
