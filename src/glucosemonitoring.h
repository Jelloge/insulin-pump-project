#ifndef GLUCOSEMONITORING_H
#define GLUCOSEMONITORING_H

#include "config.h"

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QPair>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <QTime>

QT_CHARTS_USE_NAMESPACE

class GlucoseMonitoring : public QObject {
    Q_OBJECT

public:
    explicit GlucoseMonitoring(QObject *parent,
                                 QChartView *graphView,
                                 QLabel *glucoseLabel,
                                 QPushButton *timeRangeButton);
     ~GlucoseMonitoring();
    void start();
    void stop();
    QVector<QPair<QDateTime, double>> getReadings() const;
    double getLatestReading() const;
    void cycleTimeRange();
    void saveToJson(const QString &filename);

signals:
    void newReading(double value);

private slots:
    void cleanUpOldData();

private:
    QTimer *timer;
    QVector<QPair<QDateTime, double>> glucoseReadings;

    void simulatePastReadings();
    double calculateRealisticGlucose(const QDateTime& timestamp);
    void cleanUpOldReadings();
    void generateReading();
    void setupGraph();
    void loadHistoricalData();
    void updateGraphRange();
    void updateThresholdLines();
    void updateGlucoseDisplay(double value);
    void setAlertThresholds(double low, double high); //change the high alert and low allert line
    void debugSeries();

    // UI Components
    QChartView *cgmGraphViewUI;
    QLabel *glucoseLabel;
    QPushButton *timeRangeButton;

    // Chart components
    QChart *cgmGraphView;
    QLineSeries *series;
    QLineSeries *lowerLine;
    QLineSeries *upperLine;
    QAreaSeries *targetArea;

    int currentTimeRange = 3;

    double lowAlertThreshold = 3.9;
    double highAlertThreshold = 11.1;

};

#endif // GLUCOSEMONITORING_H
