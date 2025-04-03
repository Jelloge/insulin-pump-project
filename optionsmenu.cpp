#include "optionsmenu.h"
#include "ui_optionsmenu.h"
#include "mainwindow.h"
#include "personalprofiles.h"

optionsMenu::optionsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::optionsMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

optionsMenu::~optionsMenu()
{
    delete ui;
}

void optionsMenu::on_tandemLogoButton_clicked()
{
    if (QWidget *parentWindow = this->parentWidget()) {
        this->hide();
        parentWindow->show();
    }
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




