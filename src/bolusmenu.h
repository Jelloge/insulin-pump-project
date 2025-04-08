#ifndef BOLUSMENU_H
#define BOLUSMENU_H

#include "BolusInfo.h"
#include <QWidget>
#include "boluscalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class bolusmenu; }
QT_END_NAMESPACE

class bolusmenu : public QWidget {
    Q_OBJECT

public:
    explicit bolusmenu(QWidget *parent = nullptr);
    ~bolusmenu();

private slots:
    void calculateBolus();  // Slot for calculating bolus
    void updateBolusDisplay();  // Slot for updating bolus display
    void addCarbs();  // Slot for adding carbs
    void cancelBolus();
    void logBolus(const BolusInfo& info);
    void deleteHistory();
    void on_backButton_clicked();

signals:
    void returnToMainWindow();

private:
    Ui::bolusmenu *ui;
    BolusCalculator bolusCalc;
    double totalCarbs = 0;  // Variable to accumulate total carbs
    double immediateBolus = 0;
    double extendedBolus = 0;
    double FinalBolus = 0;
    bool isBolusActive = false; // Track active bolus
    BolusInfo currentBolusInfo;

};

#endif // BOLUSMENU_H



