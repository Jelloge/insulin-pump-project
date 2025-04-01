#include "glucosemonitoring.h"
#include <cstdlib>
#include <ctime>

GlucoseMonitoring::GlucoseMonitoring(QObject *parent) : QObject(parent) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GlucoseMonitoring::generateReading);
    srand(time(nullptr)); // Seed random number generator

    simulatePastReadings();
}

void GlucoseMonitoring::simulatePastReadings() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QVector<QPair<QDateTime, double>> simulatedReadings;

    // Simulate readings every hour for the last 24 hours
    for (int i = 0; i < 24; ++i) {
        double reading = 5.5 + ((rand() % 20 - 10) / 10.0); // Simulated range 4.5 - 6.5 mmol/L
        QDateTime timestamp = currentTime.addSecs(-(i * 3600)); // Subtract hours from current time
        simulatedReadings.append(qMakePair(timestamp, reading));
    }

    // Add the simulated readings to glucoseReadings
    glucoseReadings = simulatedReadings;

    // Emit the new readings for the graph
    for (const auto &reading : simulatedReadings) {
        emit newReading(reading.second);
    }
}

void GlucoseMonitoring::start() {
    timer->start(300000); // Generate reading every 5 minutes (300,000 ms)
}

void GlucoseMonitoring::stop() {
    timer->stop();
}

void GlucoseMonitoring::cleanUpOldReadings() {
    QDateTime currentTime = QDateTime::currentDateTime();
    // Remove readings older than 3 hours
    while (!glucoseReadings.isEmpty() && glucoseReadings.first().first < currentTime.addSecs(-3 * 3600)) {
        glucoseReadings.removeFirst();
    }
}


void GlucoseMonitoring::generateReading() {
    double reading = 5.5 + ((rand() % 20 - 10) / 10.0); // Simulated range 4.5 - 6.5 mmol/L
    QDateTime timestamp = QDateTime::currentDateTime();
    glucoseReadings.append(qMakePair(timestamp, reading));

    emit newReading(reading);

    if (reading < 3.9) {
        emit lowGlucoseAlert(reading);
    }
}

QVector<QPair<QDateTime, double>> GlucoseMonitoring::getReadings() const {
    return glucoseReadings;
}

double GlucoseMonitoring::getLatestReading() const {
    if (!glucoseReadings.isEmpty()) {
        return glucoseReadings.last().second;
    }
    return -1;
}
