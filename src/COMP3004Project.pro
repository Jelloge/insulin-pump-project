QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    batterymanager.cpp \
    config.cpp \
    main.cpp \
    mainwindow.cpp \
    optionsmenu.cpp \
    personalprofiles.cpp \
    profile.cpp

HEADERS += \
    batterymanager.h \
    config.h \
    mainwindow.h \
    optionsmenu.h \
    personalprofiles.h \
    profile.h

FORMS += \
    mainwindow.ui \
    optionsmenu.ui \
    personalprofiles.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    personalProfilesv1.txt \
    profiles.json \
    ui_icons/charging.jpg

RESOURCES += \
    ui_icons/resources.qrc \
    ui_icons/resources.qrc
