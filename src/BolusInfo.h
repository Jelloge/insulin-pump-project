#ifndef BOLUSINFO_H
#define BOLUSINFO_H

#include <QJsonObject>
#include <QDateTime>

struct BolusInfo {
    double finalBolus = 0;
    double immediateBolus = 0;
    double extendedBolus = 0;
    double durationHours = 0;
    QString overrideMessage = "";  // Added for override message

    // Default constructor
    BolusInfo() = default;

    // Parameterized constructor
    BolusInfo(double final, double immediate, double extended, double duration, const QString& overrideMsg = "")
        : finalBolus(final), immediateBolus(immediate), extendedBolus(extended), durationHours(duration), overrideMessage(overrideMsg) {}

    // Method to convert BolusInfo to QJsonObject
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        obj["finalBolus"] = finalBolus;
        obj["immediateBolus"] = immediateBolus;
        obj["extendedBolus"] = extendedBolus;
        obj["durationHours"] = durationHours;
        obj["overrideMessage"] = overrideMessage;  // Include override message in JSON
        return obj;
    }
};

#endif // BOLUSINFO_H


