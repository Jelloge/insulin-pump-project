#ifndef BOLUSMENU_H
#define BOLUSMENU_H

#include "BolusInfo.h"
#include <QWidget>
#include "boluscalculator.h"
#include "glucosemonitoring.h"  

QT_BEGIN_NAMESPACE
namespace Ui { class bolusmenu; }
QT_END_NAMESPACE

class bolusmenu : public QWidget {
    Q_OBJECT

public:
    explicit bolusmenu(GlucoseMonitoring* gm, QWidget *parent = nullptr);
    ~bolusmenu();

private slots:
    void calculateBolus();  // Slot for calculating bolus
    void updateBolusDisplay();  // Slot for updating bolus display
    void addCarbs();  // Slot for adding carbs
    void cancelBolus();
    void logBolus(const BolusInfo& info);
    void deleteHistory();
    void on_backButton_clicked();
    void confirmBGInput();
    void applyOverride();
    void lloadActiveProfile();
    bool checkMaxBolus(double& bolus);

signals:
    void returnToMainWindow();

private:
    Ui::bolusmenu *ui;
    GlucoseMonitoring* glucoseMonitoring;  // store the pointer

    BolusCalculator bolusCalc;
    double totalCarbs = 0;  // Variable to accumulate total carbs
    double immediateBolus = 0;
    double extendedBolus = 0;
    double FinalBolus = 0;
    double Almost = 0;
    double CF = 1.0;
    double TARGET_BG = 5.0;
    // Backup values for override use
    double lastCarbs = 0;

    double lastBG = 0;
    double lastIOB = 0;
    double lastAlmost = 0;
    double lastTotalBolus = 0;
    double lastDeliverNowPercent = 60.0; // or a default value like 50.0
    double profileBasalRate = 0.0;
    double profileCarbRatio = 0.0;
    bool profileCarbsEnabled = false;
    double profileCorrectionFactor = 0.0;
    int profileInsulinDuration = 0;
    double profileMaxBolus = 0.0;
    QString profileName;
    double profileTargetBG = 0.0;


    bool MidDilveryCancel = false; // Track active bolus
    BolusInfo currentBolusInfo;

};

#endif // BOLUSMENU_H




