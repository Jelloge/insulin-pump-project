#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

class BolusCalculator {
public:
    double calculateTotalBolus(double carbs, double currentBG, double iob);
    void splitBolus(double totalBolus, double &immediateBolus, double &extendedBolus);
};

#endif // BOLUSCALCULATOR_H




