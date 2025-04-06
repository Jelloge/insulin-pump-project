// Stores global settings like PIN, current time, etc.
// Please use the functions in config instead of duplicating logic if you need to check or change anything related to system settings

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

void Config::updateClock() {
    currentDateTime = QDateTime::currentDateTime();
    emit clockUpdated(getFormattedDateTime());
}

QString Config::getFormattedDateTime() const {
    return currentDateTime.toString("hh:mm:ss  |  dd MMM");
}

