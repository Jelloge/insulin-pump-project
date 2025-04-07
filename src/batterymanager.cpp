#include "batterymanager.h"
#include "config.h" // To pause/resume clock

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
      chargingIcon(nullptr),
      lowBatteryWarningShown(false)
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

    // Clear the charging icon if present
    if (chargingIcon)
        chargingIcon->clear();
}

// Start full battery monitoring
void BatteryManager::start() {
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000);
}

// ysed on each page switch to ensure draining continues
void BatteryManager::startDraining() {
    isOn = true;

    if (!batteryLabel) {
        qDebug() << "[BatteryManager] No battery label set";
        return;
    }
    if (!batteryDrainTimer->isActive()) {
        batteryDrainTimer->start(3000);
    }
}

// stop draining but preserve state
void BatteryManager::stop() {
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

// kill all operations
void BatteryManager::stopAll() {
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
    if (chargingIcon) chargingIcon->clear();
}

// turn on device globally
void BatteryManager::turnOn() {
    // If already ON, do nothing
    if (isOn) return;
    isOn = true;

    // Start draining battery
    startDraining();

    // Resume clock
    Config::instance()->resumeClock();

    qDebug() << "Device turned ON";
}

// Turn off device globally
void BatteryManager::turnOff() {
    if (!isOn) return;
    isOn = false;

    // Stop battery draining and charging
    stopAll();

    // Pause the clock
    Config::instance()->pauseClock();

    qDebug() << "Device turned OFF";
}

// Start charging the battery
void BatteryManager::plugIn() {
    if (isCharging || batteryLevel >= 100) return;

    isCharging = true;
    if (chargingIcon) {
        chargingIcon->setPixmap(QPixmap(":/ui_icons/charging.jpg").scaled(20, 20, Qt::KeepAspectRatio));
    }
    if (!batteryChargeTimer->isActive()) {
        batteryChargeTimer->start(1000);
    }
}

// Stop charging
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

    // If battery > 10, reset the warning shown
    if (batteryLevel > 10) {
        lowBatteryWarningShown = false;
    }

    if (batteryLevel == 10 && !lowBatteryWarningShown) {
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: red;");
        QMessageBox::warning(nullptr, "Battery Low", "Battery is at 10%. Please plug in the charger!");
        lowBatteryWarningShown = true;
    } else if (batteryLevel < 10) {
        // Already low, keep style red
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: red;");
    } else {
        // battery > 10
        batteryLabel->setStyleSheet("font-weight: bold; font-size: 11pt; color: lightgreen;");
    }
}
