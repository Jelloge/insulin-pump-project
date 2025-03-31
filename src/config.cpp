// Stores global settings like PIN, current time, etc.
// Please use the functions in config instead of duplicating logic if you need to check or change anything related to system settings

// We could put volume / alert preferences in here too, Insulin settings like default basal/bolus thresholds, etc.

#include "config.h"

//
// Constructor: LINK TO RELEVANT UI PAGES
//

Config::Config(QWidget *settingsPage1, QWidget *settingsPage2)
    : settingsPage1(settingsPage1), settingsPage2(settingsPage2)

{
    curDateTime = QDateTime::currentDateTime();
    curPIN = -1;

//
// connecting UI elements from settings page 1
//

    QPushButton *deletePINButton = settingsPage1->findChild<QPushButton*>("DeletePINButton"); // PIN modification
    connect(deletePINButton, &QPushButton::clicked, this, &Config::deletePIN);

    QPushButton *setPINButton = settingsPage1->findChild<QPushButton*>("SetPINButton"); // PIN modification
    connect(setPINButton, &QPushButton::clicked, this, &Config::setPIN);
}

//
// time management
//

void Config::setDateTime(const QDateTime &dateTime) {
    if(curDateTime != dateTime) {
        curDateTime = dateTime;
    }
}

QDateTime Config::getDateTime() const{
    return curDateTime;
}

//
// PIN management
//

void Config::setPIN() {
    QString newPIN = settingsPage1->findChild<QTextEdit*>("NewPIN")->toPlainText();

    if (newPIN.contains(QRegularExpression("\\D"))) {
        curPIN = -1; // Invalid, contains non digit
    } else {
        curPIN = newPIN.toInt();
    }

    // Update UI feedback

    if (isPINSet()) {
        settingsPage1->findChild<QTextBrowser*>("CurrentPIN")->setText(newPIN);
        settingsPage1->findChild<QTextEdit*>("NewPIN")->setText("");
        settingsPage1->findChild<QLabel*>("ErrorNewPIN")->setText("");
    } else {
        QLabel *errorLabel = settingsPage1->findChild<QLabel*>("ErrorNewPIN");
        errorLabel->setText("Invalid PIN");
        errorLabel->setStyleSheet("color: red;");
    }
}

void Config::deletePIN() {
    curPIN = -1;
    settingsPage1->findChild<QTextBrowser*>("CurrentPIN")->setText("");
}


bool Config::isPINSet() const {
    return curPIN > 0;
}

int Config::getCurPIN() const {
    return curPIN;
}
