#include "boluscalculator.h"


const double IMMEDIATE_FRACTION = 0.6;

double BolusCalculator::calculateTotalBolus(double carbs, double currentBG, double iob) {
    double carbBolus = carbs / ICR;
    double correctionBolus = (currentBG - TARGET_BG) / CF;
    double totalBolus = carbBolus + correctionBolus - iob;
    return totalBolus > 0 ? totalBolus : 0;
}

void BolusCalculator::splitBolus(double totalBolus, double &immediateBolus, double &extendedBolus)
{

    immediateBolus = totalBolus * IMMEDIATE_FRACTION;
    extendedBolus = totalBolus * (1 - IMMEDIATE_FRACTION);


}
