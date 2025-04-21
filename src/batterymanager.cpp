#include "batterymanager.h"
#include <QMessageBox>

BatteryManager* BatteryManager::instance()
{
    static BatteryManager* _instance = new BatteryManager();
    return _instance;
}

BatteryManager::BatteryManager(QObject *parent)
    : QObject(parent),
      batteryLevel(100),
      isCharging(false),
      isOn(false),
      lowBatteryWarningShown(false),
      batteryDrainTimer(new QTimer(this)),
      batteryChargeTimer(new QTimer(this))
{
    connect(batteryDrainTimer,  &QTimer::timeout, this, &BatteryManager::drainBattery);
    connect(batteryChargeTimer, &QTimer::timeout, this, &BatteryManager::chargeBatteryStep);
}

void BatteryManager::registerLabel(QLabel *label)
{
    if (!label) return;
    if (!batteryLabels.contains(label))
        batteryLabels.append(label);
    updateUI();
}

void BatteryManager::unregisterLabel(QLabel *label)
{
    batteryLabels.removeAll(label);
}

void BatteryManager::start()
{
    isOn = true;
    updateUI();
    batteryDrainTimer->start(3000);
}

void BatteryManager::startDraining()
{
    isOn = true;
    if (!batteryDrainTimer->isActive())
        batteryDrainTimer->start(3000);
}

void BatteryManager::stop()
{
    isOn = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
}

void BatteryManager::stopAll()
{
    isOn = false;
    isCharging = false;
    batteryDrainTimer->stop();
    batteryChargeTimer->stop();
}

void BatteryManager::turnOn()
{
    const QString entered = QInputDialog::getText(nullptr, // PIN Prompt first, then power
                                tr("Unlock Pump"),
                                tr("Enter PIN:"),
                                QLineEdit::Password);

    if (!Config::instance()->verifyPin(entered)) {
        QMessageBox::critical(nullptr, tr("Wrong PIN"), tr("Access denied."));
        return;                        // abort power‑on
    }

    if (isOn) return;
    isOn = true;
    startDraining();
    Config::instance()->resumeClock();
    emit deviceTurnedOn();
    qDebug() << "Device turned ON";
}

void BatteryManager::turnOff()
{
    if (!isOn) return;
    isOn = false;
    stopAll();
    Config::instance()->pauseClock();
    emit deviceTurnedOff();
    qDebug() << "Device turned OFF";
}

void BatteryManager::plugIn()
{
    if (isCharging || batteryLevel >= 100) return;
    isCharging = true;
    if (!batteryChargeTimer->isActive())
        batteryChargeTimer->start(1000);
}

void BatteryManager::unplug()
{
    isCharging = false;
    batteryChargeTimer->stop();
}

void BatteryManager::drainBattery()
{
    if (!isCharging && isOn && batteryLevel > 0) {
        --batteryLevel;
        updateUI();
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
        ++batteryLevel;
        updateUI();
    } else if (batteryLevel >= 100) {
        batteryChargeTimer->stop();
    }
}

void BatteryManager::updateUI()
{
    for (auto &lblPtr : batteryLabels)
    {
        if (!lblPtr) continue;

        lblPtr->setText(QString("Battery: %1%").arg(batteryLevel));

        if (batteryLevel > 10) {
            lblPtr->setStyleSheet("font-weight:bold;font-size:11pt;color:lightgreen;");
            lowBatteryWarningShown = false;
        }
        else if (batteryLevel == 10 && !lowBatteryWarningShown) {
            lblPtr->setStyleSheet("font-weight:bold;font-size:11pt;color:red;");
            QMessageBox::warning(nullptr, "Battery Low",
                                 "Battery is at 10 %. Please plug in the charger!");
            lowBatteryWarningShown = true;
        }
        else { // <10
            lblPtr->setStyleSheet("font-weight:bold;font-size:11pt;color:red;");
        }
    }
}
