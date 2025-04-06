#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QLabel>

class BatteryManager : public QObject {
    Q_OBJECT

public:
    static BatteryManager* instance(); // Singleton accessor

    void start();
    void stop();
    void plugIn();
    void unplug();
    void startDraining();
    void stopAll();

    void updateLabel(QLabel *newBatteryLabel, QLabel *newChargingIcon = nullptr);

signals:
    void batteryDepleted();

private:
    explicit BatteryManager(QObject *parent = nullptr); // Private constructor

    int batteryLevel = 100;
    bool isCharging = false;
    bool isOn = false;

    QLabel *batteryLabel = nullptr;
    QLabel *chargingIcon = nullptr;
    QTimer *batteryDrainTimer = nullptr;
    QTimer *batteryChargeTimer = nullptr;

    void drainBattery();
    void chargeBatteryStep();
    void updateUI();
};

#endif // BATTERYMANAGER_H
