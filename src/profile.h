#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QTime>

class profile {

public:
    QString     profileName;
    double      basalRate = 0.0;
    int         correctionFactor = 0;
    int         carbRatio = 0;
    int         targetBG = 0;
    int         insulinDuration = 0;
    int         maxBolus = 0;
    bool        carbsEnabled = false;
    bool        isActive = false;
    QTime       profileTime;
};

#endif // PROFILE_H
