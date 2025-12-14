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

GymMainWindow::GymMainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    QString style = R"(
        QGroupBox { font-weight: bold; border: 1px solid gray; border-radius: 5px; margin-top: 10px; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px; }
        QPushButton { min-height: 25px; border-radius: 4px; border: 1px solid #888; background: #f0f0f0; }
        QPushButton:hover { background: #e0e0e0; }
    )";
    this->setStyleSheet(style);
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
