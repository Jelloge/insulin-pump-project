#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>
#include <QMetaObject>
#include <QPointer>
#include <QVector>
#include "config.h"
#include "historylogger.h"

class BatteryManager : public QObject {
    Q_OBJECT

public:
    static BatteryManager* instance(); // Singleton

    void registerLabel(QLabel *label);
    void unregisterLabel(QLabel *label);
    inline void updateLabel(QLabel *lbl) { registerLabel(lbl); }

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
    void deviceTurnedOn();
    void deviceTurnedOff();

private:
    explicit BatteryManager(QObject *parent=nullptr);

    int  batteryLevel;
    bool isCharging;
    bool isOn;
    bool lowBatteryWarningShown;

    QVector<QPointer<QLabel>> batteryLabels;

    QTimer *batteryDrainTimer;
    QTimer *batteryChargeTimer;

    void drainBattery();
    void chargeBatteryStep();
    void updateUI();
};

#endif // BATTERYMANAGER_H
