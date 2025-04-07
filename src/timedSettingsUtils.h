#ifndef TIMEDSETTINGSUTILS_H
#define TIMEDSETTINGSUTILS_H

#include <QString>
#include <QJsonArray>
#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>

QJsonArray loadTimedSettings();
bool saveTimedSettings(const QJsonArray &array);
QString getTimedSettingsFilePath();

#endif // TIMEDSETTINGSUTILS_H
