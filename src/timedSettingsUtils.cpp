#include "timedSettingsUtils.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>


QString getTimedSettingsFilePath() {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/timedSettings.json";
}

QJsonArray loadTimedSettings() {
    QFile file(getTimedSettingsFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return {};

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.isArray() ? doc.array() : QJsonArray();
}

bool saveTimedSettings(const QJsonArray &array) {
    QFile file(getTimedSettingsFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;

    file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
