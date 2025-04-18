#include "boluscalculator.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

void BolusCalculator::CalcActiveProfile() {
    QString filePath = "/home/student/.local/share/COMP3004Project/profiles.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        return;
    }

    QJsonArray profiles = doc.array();
    for (const QJsonValue& val : profiles) {
        QJsonObject obj = val.toObject();
        if (obj.contains("isActive") && obj["isActive"].toBool()) {
            if (obj.contains("carbRatio"))
                ICR = obj["carbRatio"].toDouble();
        }
    }
}

double BolusCalculator::calculateTotalBolus(double carbs,  double iob) {;
    CalcActiveProfile();
    double carbBolus = carbs / ICR;

    double totalBolus = carbBolus - iob;
    return totalBolus > 0 ? totalBolus : 0;
}

void BolusCalculator::splitBolus(double totalBolus, double deliverNowPercent, double &immediateBolus, double &extendedBolus)
{
    double nowFraction = deliverNowPercent / 100.0;
    immediateBolus = totalBolus * nowFraction;
    extendedBolus = totalBolus * (1 - nowFraction);
}
