# 基础模块（Qt 5.14.2 必需，适配所有功能）
QT += core gui widgets network

# 目标可执行文件名（与.pro文件一致）
TARGET = GMMS

# 项目类型（应用程序）
TEMPLATE = app

# 源文件列表（所有.cpp文件，顺序无关）
SOURCES += main.cpp \
           MainWindow.cpp \
           Member.cpp \
           Course.cpp \
           DataManager.cpp \
           AddEditMemberDialog.cpp \
           AddEditCourseDialog.cpp

# 头文件列表（所有.h文件，顺序无关）
HEADERS  += MainWindow.h \
            Member.h \
            Course.h \
            DataManager.h \
            AddEditMemberDialog.h \
            AddEditCourseDialog.h

# 中文显示适配（避免Qt 5中文乱码）
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
