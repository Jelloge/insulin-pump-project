#include "personalprofiles.h"
#include "ui_personalprofiles.h"

personalProfiles::personalProfiles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::personalProfiles)
    //ui(new Ui::personalProfiles)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

personalProfiles::~personalProfiles()
{
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
    ui->stackedWidget->setCurrentIndex(2);
}

