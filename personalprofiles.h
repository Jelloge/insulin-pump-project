#ifndef PERSONALPROFILES_H
#define PERSONALPROFILES_H

#include <QWidget>

namespace Ui {
class personalProfiles;
}

class personalProfiles : public QWidget
{
    Q_OBJECT

public:
    explicit personalProfiles(QWidget *parent = nullptr);
    ~personalProfiles();

private slots:
    void on_addProfileButton_clicked();
    void on_virtualKeyPressed();



    //void on_backButton_9_clicked();

    void on_checkButton_clicked();

private:
    Ui::personalProfiles *ui;
};

#endif // PERSONALPROFILES_H
