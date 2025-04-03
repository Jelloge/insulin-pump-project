#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QWidget>

namespace Ui {
class optionsMenu;
}

class optionsMenu : public QWidget
{
    Q_OBJECT

public:
    explicit optionsMenu(QWidget *parent = nullptr);
    ~optionsMenu();

private slots:
    void on_myPumpButton_clicked();

    void on_personalProfilesButton_clicked();

    //void on_addProfileButton_clicked();

    //void on_oneButton_clicked();

    //void on_virtualKeyPressed();

    void on_tandemLogoButton_clicked();

private:
    Ui::optionsMenu *ui;
};

#endif // OPTIONSMENU_H
