#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QFont>
#include <QString>
#include "config.h"
#include "batterymanager.h"

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

// feel free to modify or add more

private slots:
    void on_optionsButton_clicked();

private:
    Ui::MainWindow *ui;
    BatteryManager *batteryManager;
    Config *config;
    bool isOn;
    bool existPIN;
    QTimer *clock;
    int profNum = 0;

    bool checkingPIN();
    void changeDateTime(const QDateTime &datePlusTime2);
};
#endif // MAINWINDOW_H
