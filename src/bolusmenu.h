#ifndef BOLUSMENU_H
#define BOLUSMENU_H

#include "BolusInfo.h"
#include <QMainWindow>
#include "boluscalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calculateBolus();  // Slot for calculating bolus
    void updateBolusDisplay();  // Slot for updating bolus display
    void addCarbs();  // Slot for adding carbs
    void cancelBolus();
    void logBolus(const BolusInfo& info);
    void deleteHistory();




private:
    Ui::MainWindow *ui;
    BolusCalculator bolusCalc;
    double totalCarbs = 0;  // Variable to accumulate total carbs
    double immediateBolus = 0;
    double extendedBolus = 0;
    double FinalBolus = 0;
    bool isBolusActive = false; // Track active bolus
    BolusInfo currentBolusInfo;

};

#endif // BOLUSMENU_H



