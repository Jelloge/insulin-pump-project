#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QFont>
#include <QString>

#include "configdata.h"

#define MAX_BATT 100

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void turnOff();
    void turnOn();
    void returnHome();
    void chargeBattery();
    void chargerUnplugged();

private slots:

    void setMainClock();

    void DateTimeChanged(const QDateTime &dateTime);

    void CreateProfileClicked();

    void AddCarbsClicked();

    void AddBGClicked();

    void ConfirmBolusClicked();

    void CancelBolusEntry();

    void ConfirmBolusRejected();

    void SetDeliverySplitClicked();

    void SetDurationClicked();

    void SetDeliveryTimeClicked();


private:
    Ui::MainWindow *ui;
    bool isOn;
    bool isCharging;
    int battery;
    Config *config;
    bool existPIN;
    QTimer *clock;
    int profNum = 0;

    void batteryDrain();
    bool checkingPIN();
    void changeDateTime(const QDateTime &datePlusTime2);
};
#endif // MAINWINDOW_H
