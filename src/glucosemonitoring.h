#ifndef GLUCOSEMONITORING_H
#define GLUCOSEMONITORING_H


#include <QObject>
#include <QTimer>
#include <QVector>
#include <QDateTime>

class GlucoseMonitoring : public QObject {
    Q_OBJECT

public:
    explicit GlucoseMonitoring(QObject *parent = nullptr);
    void start();
    void stop();
    QVector<QPair<QDateTime, double>> getReadings() const;
    double getLatestReading() const;
    void simulatePastReadings();

signals:
    void newReading(double value);
    void lowGlucoseAlert(double value);

private slots:
    void cleanUpOldReadings();
    void generateReading();

private:
    QTimer *timer;
    QVector<QPair<QDateTime, double>> glucoseReadings;

};

#endif // GLUCOSEMONITORING_H
