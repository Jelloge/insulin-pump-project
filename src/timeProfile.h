#ifndef TIMEPROFILE_H
#define TIMEPROFILE_H

#include <QString>
#include <QTime>

class timeProfile {

public:
    //QString     profileName;
    double      basalRate = 0.0;
    int         correctionFactor = 0;
    int         carbRatio = 0;
    int         targetBG = 0;
    bool        isActive = false;
    QTime       profileTime;
};


#endif // TIMEPROFILE_H
