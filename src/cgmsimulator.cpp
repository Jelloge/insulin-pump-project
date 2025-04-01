#include "cgmsimulator.h"

CGMSimulator::CGMSimulator(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &CGMSimulator::generateNewReading);
}

void CGMSimulator::startSimulation() {
    m_timer->start(5000); //new reading every 5 seconds
}

void CGMSimulator::stopSimulation() {
    m_timer->stop();
}

void CGMSimulator::generateNewReading() {
    //simulate realistic glucose changes with some randomness
    double change = (m_random.generateDouble() - 0.5) * 0.4; //-0.2 to +0.2

    //trend logic
    if (!m_readings.isEmpty()) {
        double lastTrend = m_currentValue - m_readings.last();
        change += lastTrend * 0.3;
    }

    m_currentValue += change;

    //keep within bounds
    if (m_currentValue < 2.0) m_currentValue = 2.0;
    if (m_currentValue > 15.0) m_currentValue = 15.0;

    m_readings.append(m_currentValue);
    if (m_readings.size() > 288) { // Keep last 24 hours (5-min readings)
        m_readings.removeFirst();
    }

    emit newReadingAvailable(m_currentValue);
}

double CGMSimulator::currentReading() const {
    return m_currentValue;
}

QVector<double> CGMSimulator::lastReadings(int count) const {
    if (count >= m_readings.size()) return m_readings;
    return m_readings.mid(m_readings.size() - count);
}
