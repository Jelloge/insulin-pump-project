#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QLabel>

class BatteryManager : public QObject {
    Q_OBJECT

public:
    static BatteryManager* instance(); // Singleton accessor

    void updateLabel(QLabel *newBatteryLabel, QLabel *newChargingIcon = nullptr);
    void start();
    void startDraining();
    void stop();
    void stopAll();
    void turnOn();
    void turnOff();
    void plugIn();
    void unplug();

signals:
    void batteryDepleted();

private:
    explicit BatteryManager(QObject *parent = nullptr);

    int batteryLevel = 100;
    bool isCharging = false;
    bool isOn = false;
    bool lowBatteryWarningShown;

    QLabel *batteryLabel = nullptr;
    QLabel *chargingIcon = nullptr;
    QTimer *batteryDrainTimer = nullptr;
    QTimer *batteryChargeTimer = nullptr;

    void drainBattery();
    void chargeBatteryStep();
    void updateUI();
};

#endif // BATTERYMANAGER_H
