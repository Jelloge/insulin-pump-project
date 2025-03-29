// Stores global settings like PIN, current time, etc.
// Please use the functions in config instead of duplicating logic if you need to check or change anything related to system settings

#include "config.h"

Config::Config(QWidget *_settingsPage1, QWidget *_settingsPage2) : settingsPage1(_settingsPage1), settingsPage2(_settingsPage2)
{
    curDateTime = QDateTime::currentDateTime();
    curPIN = -1;

    QPushButton *deletePINButton = settingsPage1->findChild<QPushButton*>("DeletePINButton");
    connect(deletePINButton, &QPushButton::clicked, this, &Config::deletePIN);

    QPushButton *setPINButton = settingsPage1->findChild<QPushButton*>("SetPINButton");
    connect(setPINButton, &QPushButton::clicked, this, &Config::setPIN);
}

void Config::setDateTime(const QDateTime &datePlusTimeSetting) {
    if(curDateTime != datePlusTimeSetting){
        curDateTime = datePlusTimeSetting;
    }
}

QDateTime Config::getDateTime() const{
    return curDateTime;
}

void Config::setPIN() {
    QString newPIN = settingsPage1->findChild<QTextEdit*>("NewPIN")->toPlainText();
    if (newPIN.contains(QRegularExpression("\\D"))) {
        curPIN = -1;
    } else {
        curPIN = newPIN.toInt();
    }

    if(isPINSet()) {
        settingsPage1->findChild<QTextBrowser*>("CurrentPIN")->setText(newPIN);
        settingsPage1->findChild<QTextEdit*>("NewPIN")->setText("");
        settingsPage1->findChild<QLabel*>("ErrorNewPIN")->setText("");
    }else {
        settingsPage1->findChild<QLabel*>("ErrorNewPIN")->setText("Invalid PIN");
        settingsPage1->findChild<QLabel*>("ErrorNewPIN")->setStyleSheet("color: red;");
    }
}

void Config::deletePIN() {
    curPIN = -1;
    settingsPage1->findChild<QTextBrowser*>("CurrentPIN")->setText("");
}


bool Config::isPINSet() const {
    if(curPIN > 0) {
        return true;
    }
    return false;
}

int Config::getCurPIN() const {
    return curPIN;
}
