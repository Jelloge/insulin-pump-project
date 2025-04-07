#include "config.h"

Config* Config::_instance = nullptr;

Config::Config(QObject *parent)
    : QObject(parent)
{
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &Config::updateClock);
    clockTimer->start(1000);
}

Config* Config::instance() {
    if (!_instance) {
        _instance = new Config();
    }
    return _instance;
}

// Called every second to refresh time
void Config::updateClock() {
    currentDateTime = QDateTime::currentDateTime();
    emit clockUpdated(getFormattedDateTime());
}

// Return the current time as a string
QString Config::getFormattedDateTime() const {
    return currentDateTime.toString("hh:mm:ss  |  dd MMM");
}

// Pause the clock updates
void Config::pauseClock() {
    if (clockTimer->isActive()) {
        clockTimer->stop();
    }
}

// Resume the clock updates
void Config::resumeClock() {
    if (!clockTimer->isActive()) {
        clockTimer->start(1000);
    }
}
