#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QRegularExpression>
#include <QString>
#include <QTimer>

class Config : public QObject
{
    Q_OBJECT

public:
    Config(QWidget *_settingsPage1, QWidget *_settingsPage2);
    ~Config() { /* empty */ }
    bool isPINSet() const;
    int getCurPIN() const;
    QDateTime getDateTime() const;

public slots:
    void setDateTime(const QDateTime &datePlusTimeSetting);
    void setPIN();
    void deletePIN();

private:
    QWidget *settingsPage1;
    QWidget *settingsPage2;
    QDateTime curDateTime;
    int curPIN;
    bool bluetoothActive;
};

#endif // CONFIG_H
