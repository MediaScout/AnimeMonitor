QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutappdlg.cpp \
    apijimov.cpp \
    curlconnection.cpp \
    infoprovdlg.cpp \
    main.cpp \
    mainwindow.cpp \
    provider.cpp \
    rescache.cpp \
    themestyle.cpp

HEADERS += \
    aboutappdlg.h \
    apijimov.h \
    constants.h \
    curlconnection.h \
    infoprovdlg.h \
    mainwindow.h \
    provider.h \
    rescache.h \
    themestyle.h

FORMS += \
    aboutappdlg.ui \
    infoprovdlg.ui \
    mainwindow.ui

QT += network

LIBS += $$(VCPKG_X64_LIBS)/libcurl.lib
INCLUDEPATH += $$(VCPKG_X64_INCLUDES)

RC_ICONS = favicon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../build-animemonitor-Desktop_Qt_5_12_12_MSVC2017_64bit-Debug/providers.json
