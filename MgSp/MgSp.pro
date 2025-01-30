QT       += core gui
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AesEncryptionManager.cpp \
    databasemanager.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    qaesencryption.cpp \
    userdata.cpp

HEADERS += \
    AesEncryptionManager.h \
    databasemanager.h \
    login.h \
    mainwindow.h \
    qaesencryption.h \
    userdata.h

FORMS += \
        login.ui \
    mainwindow.ui

DISTFILES += \
    browser_extension/background.js \
    browser_extension/content.js \
    browser_extension/manifest.json
