// BatteryManager.h
#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <QObject>
#include <QTimer>

class BatteryManager : public QObject {
    Q_OBJECT

public:
    explicit BatteryManager(QObject *parent = nullptr);

    int getLevel() const;
    bool isCharging() const;

public slots:
    void startCharging();
    void stopCharging();
    void startDraining();
    void stopDraining();

signals:
    void batteryLevelChanged(int newLevel);
    void batteryLow();
    void batteryEmpty();
    void batteryFull();

private:
    int batteryLevel;
    bool charging;
    QTimer *chargeTimer;
    QTimer *drainTimer;
};

#endif // BATTERYMANAGER_H
