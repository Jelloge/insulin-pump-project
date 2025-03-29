#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->OnButton, SIGNAL(clicked()), this, SLOT(turnOn()));

    connect(ui->OffButton, SIGNAL(clicked()), this, SLOT(turnOff()));

    connect(ui->TandemLogo, SIGNAL(clicked()), this, SLOT(returnHomePage()));

    ui->profileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->ChargeButton, &QPushButton::clicked, this, [this]() {
        QtConcurrent::run(this, &MainWindow::chargeBattery);
    });

    connect(ui->UnplugButton, SIGNAL(clicked()), this, SLOT(unplugCharger()));

    connect(ui->OptionsButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->OptionsPage);
    });

    connect(ui->SettingsButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->SettingsPage_1);
    });

    connect(ui->DownButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->SettingsPage_2);
    });

    connect(ui->UpButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->SettingsPage_1);
    });

    connect(ui->BolusButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->BolusPage);
    });

    connect(ui->backToHome, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->HomePage);
    });

    connect(ui->BTN0, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->BolusPage);
    });

    connect(ui->enterCarbsBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->enterCarbsPage);
    });

    connect(ui->backToHome_2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->HomePage);
    });

    connect(ui->backToHome_3, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->OptionsPage);
    });

    connect(ui->backToOptionsBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->OptionsPage);
    });

    connect(ui->myPumpBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->myPumpPage);
    });

    connect(ui->backToMyPumpBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->myPumpPage);
    });

    connect(ui->personalProfilesBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->personalProfilesPage);
    });

    connect(ui->createProfilePageBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->profileNameCreatePage);
    });

    connect(ui->addCarbsBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->BolusPage);
    });

    connect(ui->backToBolusBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->BolusPage);
    });

    connect(ui->addBgBTN_2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->BolusPage);
    });

    connect(ui->addBgBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->enterBgPage);
    });

    connect(ui->confirmBolusBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->confirmBolusPage);
    });

    connect(ui->backToDeliverBTN, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->deliverBolusPage);
    });

MainWindow::~MainWindow()
{
    delete ui;
    delete Config;
}

void MainWindow::Clock() {
}

void MainWindow::turnOff() {
}

void MainWindow::turnOn() {
}

bool MainWindow::checkingPIN() {
}

void MainWindow::returnHome() {
}

void MainWindow::chargeBattery() {
}

void MainWindow::chargerUnplugged() {
}

void MainWindow::batteryDrain() {
}

void MainWindow::DateTimeChanged(){
}

void MainWindow::CreateProfileClicked(){
}


void MainWindow::AddCarbsClicked(){
}


void MainWindow::AddBGClicked(){
}


void MainWindow::ConfirmBolusClicked(){
}


void MainWindow::CancelBolusEntry(){
}



void MainWindow::ConfirmBolusRejected(){
}



void MainWindow::SetDeliverySplitClicked(){
}


void MainWindow::SetDurationClicked(){
}


void MainWindow::SetDeliveryTimeClicked(){
}

