#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

class BolusCalculator {
public:
    double calculateTotalBolus(double carbs, double iob);
    void splitBolus(double totalBolus, double deliverNowPercent, double &immediateBolus, double &extendedBolus);
    double ICR = 1.0;
    void CalcActiveProfile();

    //double correctionBolus = 0;
};

#endif // BOLUSCALCULATOR_H



