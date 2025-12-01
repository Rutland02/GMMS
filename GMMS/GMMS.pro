QT       += core gui widgets

TARGET = GMMS
TEMPLATE = app

# 源文件编码设置
CODECFORSRC = UTF-8

# 源文件
SOURCES += \
        CheckIn.cpp \
        Reservation.cpp \
        main.cpp \
        MainWindow.cpp \
        Member.cpp \
        Course.cpp \
        DataManager.cpp \
        AddEditMemberDialog.cpp \
        AddEditCourseDialog.cpp

# 头文件
HEADERS  += \
        CheckIn.h \
        MainWindow.h \
        Member.h \
        Course.h \
        DataManager.h \
        AddEditMemberDialog.h \
        AddEditCourseDialog.h \
        Reservation.h

# 资源文件（如果有）
# RESOURCES += \
#         resources.qrc

# 平台相关配置
win32 {
    CONFIG += console
}

# 编译选项
QMAKE_CXXFLAGS += -std=c++11
