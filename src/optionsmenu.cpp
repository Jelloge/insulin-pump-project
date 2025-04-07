#include "optionsmenu.h"
#include "ui_optionsmenu.h"
#include "mainwindow.h"
#include "personalprofiles.h"
#include "batterymanager.h"
#include "mainwindow.h"
#include "config.h"

optionsMenu::optionsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::optionsMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Set the battery label
    BatteryManager::instance()->updateLabel(ui->batteryLabel);

    // Clock update hookup
    connect(Config::instance(), &Config::clockUpdated, this, [=](const QString &time){
        ui->timeDateLabel->setText(time);
    });

    // Power / Battery buttons
    connect(ui->ChargeButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
    connect(ui->turnOffButton, &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOff);
    connect(ui->turnOnButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::turnOn);
    connect(ui->ChargeButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::plugIn);
    connect(ui->UnplugButton,  &QPushButton::clicked, BatteryManager::instance(), &BatteryManager::unplug);
}

optionsMenu::~optionsMenu()
{
    delete ui;
}

void optionsMenu::on_stopInsulinButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void optionsMenu::on_loadButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void optionsMenu::on_activityButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Fixes issue where Tandom Logo Button breaks battery and power buttons !
void optionsMenu::on_tandemLogoButton_clicked()
{
    if (QWidget *parentWindow = this->parentWidget()) {
        if (auto *main = qobject_cast<MainWindow *>(parentWindow)) {
            main->refreshBatteryBindings();  // 🔁 Rebind labels
        }

        this->hide();
        parentWindow->show();
    }
}

void optionsMenu::on_myPumpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void optionsMenu::on_personalProfilesButton_clicked()
{
    personalProfiles *profilesPage = new personalProfiles(this);
    profilesPage->show();
}

void optionsMenu::on_myCGMButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void optionsMenu::on_deviceSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}


void optionsMenu::on_historyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}



