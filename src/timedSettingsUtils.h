#ifndef TIMEDSETTINGSUTILS_H
#define TIMEDSETTINGSUTILS_H

#include <QString>
#include <QJsonArray>

QJsonArray loadTimedSettings();
bool saveTimedSettings(const QJsonArray &array);
QString getTimedSettingsFilePath();

#endif // TIMEDSETTINGSUTILS_H
