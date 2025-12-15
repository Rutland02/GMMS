#include "gymmainwindow.h"
#include "../core/gymdata.h"
#include "membertab.h"
#include "coursetab.h"
#include "bookingtab.h"
#include "querytab.h"
#include "statisticstab.h"
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

GymMainWindow::GymMainWindow(QWidget *parent)
    : QMainWindow(parent), isDarkTheme(false)
{
    setWindowTitle("健身房会员管理系统 (GymManager)");
    resize(1000, 700);

    data = new GymData(this);
    
    // 尝试从默认路径加载数据
    const QString dataFile = "gym_data.json";
    if (!data->loadFromJson(dataFile)) {
        QMessageBox::information(this, "信息", "未找到数据文件，将使用默认数据。");
    }

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *l = new QVBoxLayout(central);

    // 添加主题切换按钮
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    
    themeToggleBtn = new QPushButton("🌙 切换到深色主题");
    themeToggleBtn->setFixedSize(180, 35);
    connect(themeToggleBtn, &QPushButton::clicked, this, &GymMainWindow::toggleTheme);
    
    topLayout->addWidget(themeToggleBtn);
    l->addLayout(topLayout);

    tabs = new QTabWidget(this);
    l->addWidget(tabs);

    memberTab = new MemberTab(data, this);
    courseTab = new CourseTab(data, this);
    bookingTab = new BookingTab(data, this);
    queryTab = new QueryTab(data, this);
    statisticsTab = new StatisticsTab(data, this);

    tabs->addTab(memberTab, "👤 会员管理");
    tabs->addTab(courseTab, "📅 课程管理");
    tabs->addTab(bookingTab, "📝 预约与签到");
    tabs->addTab(queryTab, "📊 数据记录");
    tabs->addTab(statisticsTab, "📈 数据统计");

    applyStyles();
}

void GymMainWindow::applyStyles() {
    QString style = isDarkTheme ? getDarkTheme() : getLightTheme();
    this->setStyleSheet(style);
}

QString GymMainWindow::getLightTheme() {
    return R"(
        /* 主窗口样式 */
        QMainWindow { background-color: #f5f5f5; }
        
        /* 标签页样式 */
        QTabWidget { background-color: #ffffff; border-radius: 8px; margin: 10px; }
        QTabBar { background-color: #ffffff; }
        QTabBar::tab { 
            background-color: #ffffff; 
            color: #666666; 
            padding: 10px 20px; 
            border: none; 
            border-bottom: 2px solid transparent;
            border-radius: 4px 4px 0 0;
            min-width: 100px;
        }
        QTabBar::tab:selected { 
            background-color: #ffffff; 
            color: #2196F3; 
            border-bottom: 2px solid #2196F3;
            font-weight: bold;
        }
        QTabBar::tab:hover { background-color: #f0f0f0; }
        
        /* 分组框样式 */
        QGroupBox { 
            font-weight: bold; 
            border: 1px solid #e0e0e0; 
            border-radius: 8px; 
            margin-top: 15px; 
            padding: 10px; 
            background-color: #ffffff; 
        }
        QGroupBox::title { 
            subcontrol-origin: margin; 
            left: 15px; 
            padding: 0 8px; 
            color: #333333;
        }
        
        /* 按钮样式 */
        QPushButton { 
            min-height: 30px; 
            border-radius: 6px; 
            border: none; 
            background-color: #2196F3; 
            color: white; 
            font-weight: bold;
            padding: 5px 15px;
        }
        QPushButton:hover { 
            background-color: #1976D2; 
        }
        QPushButton:pressed { 
            background-color: #0D47A1; 
        }
        
        /* 输入框样式 */
        QLineEdit, QDateEdit, QComboBox, QSpinBox, QDoubleSpinBox { 
            border: 1px solid #e0e0e0; 
            border-radius: 4px; 
            padding: 6px 10px; 
            background-color: #ffffff; 
        }
        QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { 
            border-color: #2196F3; 
            outline: none;
        }
        
        /* 表格样式 */
        QTableWidget { 
            border: 1px solid #e0e0e0; 
            border-radius: 4px; 
            background-color: #ffffff; 
        }
        QHeaderView::section { 
            background-color: #f5f5f5; 
            color: #333333; 
            font-weight: bold; 
            border: none; 
            padding: 8px; 
        }
        QTableWidget::item { 
            padding: 8px; 
            border-bottom: 1px solid #f0f0f0;
        }
        QTableWidget::item:selected { 
            background-color: #E3F2FD; 
            color: #2196F3;
        }
        
        /* 文本编辑框样式 */
        QTextEdit { 
            border: 1px solid #e0e0e0; 
            border-radius: 4px; 
            background-color: #ffffff; 
            padding: 8px;
        }
        QTextEdit:focus { 
            border-color: #2196F3; 
            outline: none;
        }
        
        /* 下拉框样式 */
        QComboBox { 
            selection-background-color: #2196F3; 
            color: #333333;
        }
        QComboBox QAbstractItemView { 
            background-color: #ffffff; 
            border: 1px solid #e0e0e0;
            border-radius: 4px;
        }
        QComboBox QAbstractItemView::item:hover { 
            background-color: #f0f0f0; 
        }
    )";
}

QString GymMainWindow::getDarkTheme() {
    return R"(
        /* 基础组件样式 */
        QWidget { background-color: #1E1E1E; color: #E0E0E0; }
        
        /* 主窗口样式 */
        QMainWindow { background-color: #121212; }
        
        /* 标签页样式 */
        QTabWidget { background-color: #1E1E1E; border-radius: 8px; margin: 10px; }
        QTabBar { background-color: #1E1E1E; }
        QTabBar::tab { 
            background-color: #1E1E1E; 
            color: #B0B0B0; 
            padding: 10px 20px; 
            border: none; 
            border-bottom: 2px solid transparent;
            border-radius: 4px 4px 0 0;
            min-width: 100px;
        }
        QTabBar::tab:selected { 
            background-color: #1E1E1E; 
            color: #4FC3F7; 
            border-bottom: 2px solid #4FC3F7;
            font-weight: bold;
        }
        QTabBar::tab:hover { background-color: #2A2A2A; }
        
        /* 分组框样式 */
        QGroupBox { 
            font-weight: bold; 
            border: 1px solid #333333; 
            border-radius: 8px; 
            margin-top: 15px; 
            padding: 10px; 
            background-color: #1E1E1E; 
        }
        QGroupBox::title { 
            subcontrol-origin: margin; 
            left: 15px; 
            padding: 0 8px; 
            color: #B0B0B0;
        }
        
        /* 按钮样式 */
        QPushButton { 
            min-height: 30px; 
            border-radius: 6px; 
            border: none; 
            background-color: #4FC3F7; 
            color: #121212; 
            font-weight: bold;
            padding: 5px 15px;
        }
        QPushButton:hover { 
            background-color: #29B6F6; 
        }
        QPushButton:pressed { 
            background-color: #03A9F4; 
        }
        
        /* 输入框样式 */
        QLineEdit, QDateEdit, QComboBox, QSpinBox, QDoubleSpinBox { 
            border: 1px solid #333333; 
            border-radius: 4px; 
            padding: 6px 10px; 
            background-color: #2A2A2A; 
            color: #E0E0E0;
        }
        QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { 
            border-color: #4FC3F7; 
            outline: none;
        }
        
        /* 表格样式 */
        QTableWidget { 
            border: 1px solid #333333; 
            border-radius: 4px; 
            background-color: #1E1E1E; 
        }
        QHeaderView::section { 
            background-color: #2A2A2A; 
            color: #B0B0B0; 
            font-weight: bold; 
            border: none; 
            padding: 8px; 
        }
        QTableWidget::item { 
            padding: 8px; 
            border-bottom: 1px solid #333333;
            color: #E0E0E0;
        }
        QTableWidget::item:selected { 
            background-color: #37474F; 
            color: #4FC3F7;
        }
        
        /* 文本编辑框样式 */
        QTextEdit { 
            border: 1px solid #333333; 
            border-radius: 4px; 
            background-color: #2A2A2A; 
            padding: 8px;
            color: #E0E0E0;
        }
        QTextEdit:focus { 
            border-color: #4FC3F7; 
            outline: none;
        }
        
        /* 标签样式 */
        QLabel { 
            color: #E0E0E0;
        }
        
        /* 下拉框样式 */
        QComboBox { 
            selection-background-color: #4FC3F7; 
            color: #E0E0E0;
        }
        QComboBox QAbstractItemView { 
            background-color: #2A2A2A; 
            border: 1px solid #333333;
            border-radius: 4px;
            color: #E0E0E0;
        }
        QComboBox QAbstractItemView::item:hover { 
            background-color: #37474F; 
        }
        
        /* 日期选择器样式 */
        QDateEdit QAbstractItemView { 
            background-color: #2A2A2A; 
            border: 1px solid #333333;
            border-radius: 4px;
        }
        QDateEdit QAbstractItemView::item { 
            color: #E0E0E0;
        }
        QDateEdit QAbstractItemView::item:hover { 
            background-color: #37474F; 
        }
        QCalendarWidget QAbstractItemView { 
            background-color: #2A2A2A; 
        }
        QCalendarWidget QWidget { 
            color: #E0E0E0;
        }
        QCalendarWidget QAbstractItemView::item:selected { 
            background-color: #4FC3F7;
            color: #121212;
        }
        QCalendarWidget QCalendarDayOfWeekRow { 
            background-color: #2A2A2A; 
        }
    )";
}

void GymMainWindow::toggleTheme() {
    isDarkTheme = !isDarkTheme;
    applyStyles();
    
    if (isDarkTheme) {
        themeToggleBtn->setText("☀️ 切换到浅色主题");
    } else {
        themeToggleBtn->setText("🌙 切换到深色主题");
    }
}

void GymMainWindow::closeEvent(QCloseEvent *event) {
    const QString dataFile = "gym_data.json";
    if (data->saveToJson(dataFile)) {
        QMessageBox::information(this, "成功", "数据已保存！");
    } else {
        QMessageBox::warning(this, "警告", "数据保存失败！");
    }
    event->accept();
}
