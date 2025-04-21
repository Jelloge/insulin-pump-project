#include "config.h"

Config* Config::_instance = nullptr;

Config::Config(QObject *parent)
    : QObject(parent)
{
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &Config::updateClock);
    clockTimer->start(1000);
}

QString Config::timeFormat = "24";  // Default is 24-hour format
QString Config::dateFormat = "dd MMM";  // Default is day-month format

int Config::siteReminderDays = 3;
QString Config::siteReminderTime = "12:00";
bool Config::siteReminderEnabled = false;

bool Config::tempBasalRateEnabled = false;
bool Config::controlIQEnabled = false;

double Config::lowBGThreshold = 7.0;
int Config::lowBGRepeatDelay = 15;
double Config::highBGThreshold = 11.0;
int Config::highBGRepeatDelay = 120;
int Config::afterBolusTime = 90;
QStringList Config::missedMealDays = {};
QString Config::missedMealStartTime = "08:00";
QString Config::missedMealEndTime = "10:30";
QString Config::soundVolume = "High";


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
    QString timeFormatStr = (timeFormat == "24") ? "HH:mm" : "hh:mm AP"; // 12-hour or 24-hour format
    return currentDateTime.toString(timeFormatStr + "  |  " + dateFormat);
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

void Config::setTempBasalRatePercentage(int percentage) {
    tempBasalRatePercentage = percentage;
}

int Config::getTempBasalRatePercentage() const {
    return tempBasalRatePercentage;
}

void Config::setTempBasalDuration(int minutes) {
    tempBasalDurationMinutes = minutes;
}

int Config::getTempBasalDuration() const {
    return tempBasalDurationMinutes;
}

void Config::setWeight(double w, const QString &unit) {
    weight = w;
    weightUnit = unit;
}

double Config::getWeight() const {
    return weight;
}

void Config::setWeightUnit(const QString &unit) {
    weightUnit = unit;
}

QString Config::getWeightUnit() const {
    return weightUnit;
}

void Config::setTotalDailyInsulin(double val) {
    totalDailyInsulin = val;
}

double Config::getTotalDailyInsulin() const {
    return totalDailyInsulin;
}





