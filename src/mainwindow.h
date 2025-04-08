#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionsmenu.h"
#include "config.h"
#include "batterymanager.h"
#include "glucosemonitoring.h"
#include "bolusmenu.h"

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QFont>
#include <QString>

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
    void refreshBatteryBindings();

private slots:
    void on_optionsButton_clicked();
    void on_bolusButton_clicked();

private:
    Ui::MainWindow *ui;
    optionsMenu *options = nullptr;
    bolusmenu *bolusPage = nullptr;
    BatteryManager *batteryManager;
    Config *config;
    GlucoseMonitoring *glucoseMonitoring;
    
    bool isOn;
    bool existPIN;
    QTimer *clock;
    int profNum = 0;
    bool checkingPIN();
    void changeDateTime(const QDateTime &datePlusTime2);
};
#endif // MAINWINDOW_H
