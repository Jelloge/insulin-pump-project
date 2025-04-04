#include "personalprofiles.h"
#include "ui_personalprofiles.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>


personalProfiles::personalProfiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::personalProfiles),
    newProfile(new profile())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Connecting letter and number buttons
    connect(ui->aButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->bButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->cButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->dButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->eButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->gButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->hButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->iButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->jButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->kButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->lButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->mButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->nButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->oButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->pButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->qButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->rButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->tButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->uButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->vButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->wButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->xButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->yButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->zButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->oneButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->twoButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->threeButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fourButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->fiveButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sixButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->sevenButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->eightButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->nineButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);
    connect(ui->zeroButton, &QPushButton::clicked, this, &personalProfiles::on_virtualKeyPressed);

    // Connecting space and delete buttons
    connect(ui->spaceButton, &QPushButton::clicked, this, [=]() {
        ui->profileNameEdit->insert(" ");
    });

    connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {
        QString text = ui->profileNameEdit->text();
        text.chop(1);
        ui->profileNameEdit->setText(text);
    });

}

personalProfiles::~personalProfiles()
{
    delete newProfile;
    delete ui;
}

void personalProfiles::on_addProfileButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void personalProfiles::on_virtualKeyPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
       if (!button) return;

       QString key = button->text();
       ui->profileNameEdit->insert(key);
}

void personalProfiles::on_checkButton_clicked()
{
    // Get the Profile name
    QString profileName = ui->profileNameEdit->text().trimmed();

    if (profileName.isEmpty()) {
            QMessageBox::warning(this, "Missing Data", "Please enter a profile name.");
            return;
    }

    // Save name in a Profile object
    newProfile->profileName = profileName;
    //QMessageBox::information(this, "Name Saved", "Profile name saved: " + profileName);

    ui->stackedWidget->setCurrentIndex(2);
}

void personalProfiles::on_timedSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void personalProfiles::on_bolusSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void personalProfiles::on_basalRateButton_clicked()
{
    bool ok = false;
    double basalRate = QInputDialog::getDouble(
        this,
        tr("Enter Basal Rate"),
        tr("Range: 0.1 to 15.0 units/hr"),
        0.10,       // initial
        0.1,        // min
        15.0,       // max
        2,          // decimals
        &ok
    );

    if (ok) {
        newProfile->basalRate = basalRate;
        ui->basalRateButton->setText(QString::number(basalRate, 'f', 2) + " units/hr");
    }
}

void personalProfiles::on_correctionFactorButton_clicked()
{
    bool ok = false;
    int mgdl = QInputDialog::getInt(
       this,
       tr("Enter Correction Factor"),
       tr("Range: 1 unit:1 mg/dL to 1 unit:600 mg/dL"),
       1,       // initial
       1,       // min
       600,     // max
       1,       // step
       &ok
   );

    if (ok) {
        newProfile->correctionFactor = mgdl;
        ui->correctionFactorButton->setText(QString("1 unit:%1 mg/dL").arg(mgdl));
    }
}

void personalProfiles::on_carbRatioButton_clicked()
{
    bool ok = false;
    int gram = QInputDialog::getInt(
       this,
       tr("Enter Carb Ratio"),
       tr("Range: 1 unit:1 gram to 1 unit:300 grams"),
       1,       // initial
       1,       // min
       300,     // max
       1,       // step
       &ok
   );

    if (ok) {
        newProfile->carbRatio = gram;
        ui->carbRatioButton->setText(QString("1 unit:%1 g").arg(gram));
    }
}

void personalProfiles::on_targetBGButton_clicked()
{
    bool ok = false;
    int targetBG = QInputDialog::getInt(
       this,
       tr("Enter Target Blood Glucose"),
       tr("Range: 70  mg/dL to 250 mg/dL"),
       70,      // initial
       70,      // min
       250,     // max
       1,       // step
       &ok
   );

    if (ok) {
        newProfile->targetBG = targetBG;
        ui->targetBGButton->setText(QString("%1 mg/dL").arg(targetBG));
    }
}

void personalProfiles::on_checkButton_3_clicked()
{
    double br     = newProfile->basalRate;
    int    mgdl   = newProfile->correctionFactor;
    int    grams  = newProfile->carbRatio;
    int    tBG    = newProfile->targetBG;

    QString msg = QString("Basal Rate: %1 units/hr\n"
                         "Correction Factor: 1 unit:%2 mg/dL\n"
                         "Carb Ratio: 1 unit:%3 g\n"
                         "Target BG: %4 mg/dL")
                    .arg(br)
                    .arg(mgdl)
                    .arg(grams)
                    .arg(tBG);


    QMessageBox::StandardButton reply = QMessageBox::information(
        this,
        "Confirm Settings?",
        msg,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Ok) {
        // Proceed with saving or next step
    } else {
        // User canceled
    }
}


void personalProfiles::on_insulinDurationButton_clicked()
{
    bool ok = false;
    int hours = QInputDialog::getInt(
        this,
        "Insulin Duration",
        "Hours (2–8):",
        5,
        2,
        8,
        1,
        &ok);
    if (!ok) return;

    int minutes = QInputDialog::getInt(
        this,
        "Insulin Duration",
        "Minutes:",
        0,
        0,
        59,
        1,
        &ok);
    if (!ok) return;

    int totalMinutes = hours * 60 + minutes;
    newProfile->insulinDuration = totalMinutes;
    ui->insulinDurationButton->setText(QString("%1h %2m").arg(hours).arg(minutes));
}

void personalProfiles::on_maxBolusButton_clicked()
{
    bool ok = false;
    int maxBolus = QInputDialog::getInt(
       this,
       tr("Enter Max Bolus"),
       tr("Range: 1 to 25 units"),
       10,      // initial
       1,       // min
       25,      // max
       1,       // step
       &ok
   );

    if (ok) {
        newProfile->maxBolus = maxBolus;
        ui->maxBolusButton->setText(QString("%1 units").arg(maxBolus));
    }
}

