#ifndef GLUCOSETREND_H
#define GLUCOSETREND_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <algorithm>
#include <QAreaSeries>
#include <QVBoxLayout>

QT_CHARTS_USE_NAMESPACE

class GlucoseTrendWidget : public QWidget {
    Q_OBJECT
public:
    GlucoseTrendWidget(QWidget *parent = nullptr);
    void updateReadings(const QVector<double> &readings);

private:
    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
};

#endif // GLUCOSETREND_H
