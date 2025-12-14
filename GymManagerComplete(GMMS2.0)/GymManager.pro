QT += core gui widgets charts
CONFIG += c++17

TEMPLATE = app
TARGET = GymManager

INCLUDEPATH += model core ui

SOURCES += \
    main.cpp \
    model/member.cpp \
    model/course.cpp \
    model/checkin.cpp \
    core/gymdata.cpp \
    ui/gymmainwindow.cpp \
    ui/membertab.cpp \
    ui/coursetab.cpp \
    ui/bookingtab.cpp \
    ui/querytab.cpp \
    ui/statisticstab.cpp

HEADERS += \
    model/member.h \
    model/course.h \
    model/checkin.h  \
    core/gymdata.h \
    ui/gymmainwindow.h \
    ui/membertab.h \
    ui/coursetab.h \
    ui/bookingtab.h \
    ui/querytab.h \
    ui/statisticstab.h
