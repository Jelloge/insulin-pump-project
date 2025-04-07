#include "testcgm.h"
#include <QDebug>
#include <QRandomGenerator>

TestCGM::TestCGM(GlucoseMonitoring* glucoseMonitor, QObject *parent)
    : QObject(parent), m_glucoseMonitor(glucoseMonitor)
{
    //connect(&m_testTimer, &QTimer::timeout, this, &TestHarness::accelerateTime);
}

void TestCGM::startTest(TestScenario scenario, int durationHours)
{
    m_currentScenario = scenario;
    m_remainingTestSeconds = durationHours * 3600;

    qDebug() << "Starting test scenario:" << scenario << "for" << durationHours << "hours";
    qDebug() << "Time acceleration: 1 real second =" << m_timeAcceleration << "test seconds";

    // Start with a clean slate
    m_glucoseMonitor->stop();

    // Generate initial readings appropriate for scenario
    QVector<QPair<QDateTime, double>> initialReadings;
    QDateTime currentTime = QDateTime::currentDateTime();

    // Generate 24 hours of historical data matching the scenario
    for (int i = 24*12; i > 0; i--) { // 24 hours * 12 readings/hour
        double value;
        switch(scenario) {
        case HypoglycemiaEvent:
            value = 3.5 + (QRandomGenerator::global()->generateDouble() * 1.5); // 3.5-5.0
            break;
        case HyperglycemiaEvent:
            value = 10.0 + (QRandomGenerator::global()->generateDouble() * 5.0); // 10.0-15.0
            break;
        case MealEvent:
            // Simulate pre-meal levels
            value = 4.5 + (QRandomGenerator::global()->generateDouble() * 2.0); // 4.5-6.5
            break;
        case ExerciseEvent:
            value = 5.0 + (QRandomGenerator::global()->generateDouble() * 3.0); // 5.0-8.0
            break;
        case DawnPhenomenon:
            // Lower at night, higher in morning
            {
                int hour = currentTime.addSecs(-i*300).time().hour();
                value = (hour >= 22 || hour < 6) ?
                    4.0 + (QRandomGenerator::global()->generateDouble() * 1.5) : // Night: 4.0-5.5
                    5.5 + (QRandomGenerator::global()->generateDouble() * 2.0); // Day: 5.5-7.5
            }
            break;
        case ComboScenario:
            // Alternating patterns
            {
                int hour = currentTime.addSecs(-i*300).time().hour();
                if (hour % 6 < 2) {
                    // Meal-like periods
                    value = 5.0 + (QRandomGenerator::global()->generateDouble() * 4.0);
                } else if (hour % 6 < 4) {
                    // Exercise-like periods
                    value = 4.0 + (QRandomGenerator::global()->generateDouble() * 3.0);
                } else {
                    // Normal variation
                    value = 4.5 + (QRandomGenerator::global()->generateDouble() * 3.0);
                }
            }
            break;
        default: // NormalVariation
            value = 4.5 + (QRandomGenerator::global()->generateDouble() * 3.0); // 4.5-7.5
        }

        initialReadings.prepend(qMakePair(currentTime.addSecs(-i*300), value));
    }

    // Replace the monitor's readings with our test data
    m_glucoseMonitor->setTestData(initialReadings);

    // Start the accelerated test
    m_testTimer.start(1000); // Update every real-world second
    m_glucoseMonitor->start();
}

void TestCGM::stopTest()
{
    m_testTimer.stop();
    m_glucoseMonitor->stop();
}

void TestCGM::accelerateTime()
{
    // Generate multiple readings based on our time acceleration
    int readingsToGenerate = m_timeAcceleration / 300; // Number of 5-minute intervals in accelerated time

    for (int i = 0; i < readingsToGenerate; i++) {
        if (m_remainingTestSeconds <= 0) {
            stopTest();
            emit testCompleted();
            return;
        }

        // Generate appropriate reading based on scenario
        switch(m_currentScenario) {
        case HypoglycemiaEvent:
            generateHypoglycemiaReading();
            break;
        case HyperglycemiaEvent:
            generateHyperglycemiaReading();
            break;
        case MealEvent:
            generateMealReading();
            break;
        case ExerciseEvent:
            generateExerciseReading();
            break;
        case DawnPhenomenon:
            generateDawnPhenomenonReading();
            break;
        case StressEvent:
            generateStressReading();
            break;
        case ComboScenario:
            generateComboReading();
            break;
        default:
            generateNormalReading();
        }

        m_remainingTestSeconds -= 300; // Subtract 5 minutes
    }
}

// Implement all the generate*Reading() methods similarly to this pattern:
void TestCGM::generateNormalReading()
{
    static double lastValue = m_glucoseMonitor->getLatestReading();
    if (lastValue < 0) lastValue = 5.5;

    // Small random change
    double change = (QRandomGenerator::global()->generateDouble() - 0.5) * 0.3;
    lastValue += change;

    // Keep within normal bounds
    if (lastValue < 4.0) lastValue = 4.0;
    if (lastValue > 8.0) lastValue = 8.0;

    m_glucoseMonitor->addTestReading(QDateTime::currentDateTime(), lastValue);
}

void TestCGM::generateHypoglycemiaReading()
{
    static double lastValue = m_glucoseMonitor->getLatestReading();
    if (lastValue < 0) lastValue = 4.0;

    // Trend downward with occasional drops
    double change = -0.1;
    if (QRandomGenerator::global()->generateDouble() < 0.2) {
        change -= QRandomGenerator::global()->generateDouble() * 0.5;
    }

    lastValue += change;

    // Ensure we get some low values
    if (lastValue < 3.0) lastValue = 3.5;
    if (lastValue > 5.0) lastValue = 4.5;

    m_glucoseMonitor->addTestReading(QDateTime::currentDateTime(), lastValue);
}

// Implement similar methods for other scenarios...
