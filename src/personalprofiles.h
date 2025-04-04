#ifndef PERSONALPROFILES_H
#define PERSONALPROFILES_H

#include <QWidget>
#include "profile.h"

namespace Ui {
class personalProfiles;
}

class personalProfiles : public QWidget
{
    Q_OBJECT

public:
    explicit personalProfiles(QWidget *parent = nullptr);
    ~personalProfiles();

    profile *newProfile;

private slots:
    void on_addProfileButton_clicked();
    void on_virtualKeyPressed();
    void on_checkButton_clicked();


    void on_basalRateButton_clicked();

    void on_timedSettingsButton_clicked();

    void on_correctionFactorButton_clicked();

    void on_carbRatioButton_clicked();

    void on_targetBGButton_clicked();

    void on_checkButton_3_clicked();

    void on_insulinDurationButton_clicked();

    void on_bolusSettingsButton_clicked();

    void on_maxBolusButton_clicked();

private:
    Ui::personalProfiles *ui;
};

#endif // PERSONALPROFILES_H
