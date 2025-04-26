QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boluscalculator.cpp \
    bolusmenu.cpp \
    glucosemonitoring.cpp \
    main.cpp \
    mainwindow.cpp \
    optionsmenu.cpp \
    personalprofiles.cpp \
    timedSettingsUtils.cpp \    
    batterymanager.cpp \
    config.cpp \
    timedsettingsprofiles.cpp

HEADERS += \
    BolusInfo.h \
    boluscalculator.h \
    bolusmenu.h \
    glucosemonitoring.h \
    mainwindow.h \
    optionsmenu.h \
    personalprofiles.h \
    profile.h \
    timeProfile.h \
    timedSettingsUtils.h \
    timedsettingsprofiles.h \
    batterymanager.h \
    config.h


FORMS += \
    bolusmenu.ui \
    mainwindow.ui \
    optionsmenu.ui \
    personalprofiles.ui \
    timedsettingsprofiles.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ui_icons.qrc \
    ui_icons/resources.qrc

DISTFILES += \
    personalProfilesv1.txt \
    profiles.json \
    ui_icons/back.png \
    ui_icons/charging.jpg \
    ui_icons/check.png \
    ui_icons/close.png \
    ui_icons/graydiamond.png \
    ui_icons/letter-b.png \
    ui_icons/plus.png

