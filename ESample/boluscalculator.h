#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

class BolusCalculator {
public:
    double calculateTotalBolus(double carbs, double currentBG, double iob);
    void splitBolus(double totalBolus, double &immediateBolus, double &extendedBolus);
    static constexpr double ICR = 1.0;
    static constexpr double CF = 1.0;
    static constexpr double TARGET_BG = 5.0;
};

#endif // BOLUSCALCULATOR_H




