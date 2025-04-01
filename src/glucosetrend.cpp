#include "glucosetrend.h"

GlucoseTrendWidget::GlucoseTrendWidget(QWidget *parent) : QWidget(parent)
{
    //create chart
    m_chart = new QChart();
    m_chart->setTitle("Glucose Trend");
    m_chart->legend()->hide();

    //create series
    m_axisX = new QValueAxis();
    m_axisX->setTitleText("Time (minutes)");
    m_axisX->setLabelFormat("%d");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    m_axisY = new QValueAxis();
    m_axisY->setTitleText("Glucose (mmol/L)");
    m_axisY->setLabelFormat("%.1f");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    // Safe range area
    QAreaSeries *safeRange = new QAreaSeries(
        new QLineSeries(), // upper boundary (5.5)
        new QLineSeries()  // lower boundary (3.9)
    );
    safeRange->setColor(QColor(200, 255, 200, 100));
    m_chart->addSeries(safeRange);
    safeRange->attachAxis(m_axisX);
    safeRange->attachAxis(m_axisY);

    // Create chart view
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    setLayout(layout);
}

void GlucoseTrendWidget::updateReadings(const QVector<double> &readings) {
    m_series->clear();

    if (readings.isEmpty()) return;

    // Find min/max for Y axis
    auto [minIt, maxIt] = std::minmax_element(readings.begin(), readings.end());
    double minValue = *minIt - 1.0;
    double maxValue = *maxIt + 1.0;
    if (minValue < 2.0) minValue = 2.0;
    if (maxValue > 15.0) maxValue = 15.0;

    // Update Y axis
    m_axisY->setRange(minValue, maxValue);

    // Update X axis
    m_axisX->setRange(0, readings.size() - 1);

    // Add data points
    for (int i = 0; i < readings.size(); ++i) {
        m_series->append(i, readings[i]);
    }

    // Update safe range area
    QAreaSeries *safeRange = qobject_cast<QAreaSeries*>(m_chart->series().at(1));
    if (safeRange) {
        QLineSeries *upper = safeRange->upperSeries();
        QLineSeries *lower = safeRange->lowerSeries();
        upper->clear();
        lower->clear();
        for (int i = 0; i < readings.size(); ++i) {
            upper->append(i, 5.5);
            lower->append(i, 3.9);
        }
    }
}
