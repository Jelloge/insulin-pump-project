#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QLabel>

class BatteryManager : public QObject {
    Q_OBJECT

public:

    BatteryManager(QObject *parent, QLabel *batteryLabel, QLabel *chargingIcon = nullptr);
    explicit BatteryManager(QObject *parent = nullptr, QLabel *label = nullptr);

    void start();
    void stop();
    void plugIn();
    void unplug();
    void startDraining();
    void stopAll();

signals:
    void batteryDepleted();

private:
    int batteryLevel;
    bool isCharging;
    bool isOn;

    QLabel *batteryLabel;
    QLabel *chargingIcon;
    QTimer *batteryDrainTimer;
    QTimer *batteryChargeTimer;

    void drainBattery();
    void chargeBatteryStep();
    void updateUI();
};

#endif // BATTERYMANAGER_H
