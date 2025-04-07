#ifndef TESTCGM_H
#define TESTCGM_H
#include <QObject>
#include <QTimer>
#include "glucosemonitoring.h"


class TestCGM : public QObject
{
    Q_OBJECT

public:
    TestCGM(GlucoseMonitoring* glucoseMonitor, QObject *parent = nullptr);

    enum TestScenario {
       NormalVariation,
       HypoglycemiaEvent,
       HyperglycemiaEvent,
       MealEvent,
       ExerciseEvent,
       DawnPhenomenon,
       StressEvent,
       ComboScenario
   };

   void startTest(TestScenario scenario, int durationHours);
   void stopTest();

signals:
   void testCompleted();

private slots:
   void accelerateTime();

private:
   GlucoseMonitoring* m_glucoseMonitor;
   QTimer m_testTimer;
   int m_timeAcceleration = 60; // 1 real second = 60 test seconds
   TestScenario m_currentScenario;
   int m_remainingTestSeconds;

   void generateNormalReading();
   void generateHypoglycemiaReading();
   void generateHyperglycemiaReading();
   void generateMealReading();
   void generateExerciseReading();
   void generateDawnPhenomenonReading();
   void generateStressReading();
   void generateComboReading();
};


#endif // TESTCGM_H
