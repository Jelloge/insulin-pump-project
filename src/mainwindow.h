#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QFont>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QAreaSeries>

#include "glucosemonitoring.h"
#include "config.h"
#include "batterymanager.h"

using namespace QtCharts;

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

// feel free to modify or add more

private slots:
    void CreateProfileClicked();
    void AddCarbsClicked();
    void AddBGClicked();
    void ConfirmBolusClicked();
    void CancelBolusEntry();
    void ConfirmBolusRejected();
    void SetDeliverySplitClicked();
    void SetDurationClicked();
    void SetDeliveryTimeClicked();
    //
    void setupGraph();
    void updateGraph(double value);
    void onLowGlucoseAlert(double value);

    //

private:
    Ui::MainWindow *ui;
    BatteryManager *batteryManager;
    Config *config;
    bool isOn;
    bool existPIN;
    QTimer *clock;
    int profNum = 0;

    //
    GlucoseMonitoring *glucoseMonitoring;
    QChart *cgmGraphView;
    QLineSeries *series;
    QLineSeries *lowerLine;
    QLineSeries *upperLine;
    QAreaSeries *targetArea;
    //

    bool checkingPIN();
    void changeDateTime(const QDateTime &datePlusTime2);
};
#endif // MAINWINDOW_H
