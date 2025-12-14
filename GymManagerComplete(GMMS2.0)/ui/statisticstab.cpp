#include "statisticstab.h"
#include "../core/gymdata.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QHBoxLayout>
#include <QStringList>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QApplication>
#include <QFont>
#include <QBarCategoryAxis>
using namespace QtCharts;

StatisticsTab::StatisticsTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 日期范围选择
    QGroupBox *dateGroup = new QGroupBox("统计时间范围");
    QHBoxLayout *dateLayout = new QHBoxLayout(dateGroup);
    
    startDateEdit = new QDateEdit();
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDate(QDate::currentDate().addDays(-30)); // 默认显示最近30天
    
    endDateEdit = new QDateEdit();
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDate(QDate::currentDate());
    
    updateBtn = new QPushButton("更新数据");
    
    dateLayout->addWidget(new QLabel("开始日期:"));
    dateLayout->addWidget(startDateEdit);
    dateLayout->addWidget(new QLabel("结束日期:"));
    dateLayout->addWidget(endDateEdit);
    dateLayout->addWidget(updateBtn);
    dateLayout->addStretch();
    
    mainLayout->addWidget(dateGroup);
    
    // 统计概览
    QGroupBox *overviewGroup = new QGroupBox("统计概览");
    statsLayout = new QGridLayout(overviewGroup);
    
    // 会员统计
    statsLayout->addWidget(new QLabel("总会员数:"), 0, 0);
    totalMembersLabel = new QLabel("0");
    totalMembersLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    statsLayout->addWidget(totalMembersLabel, 0, 1);
    
    statsLayout->addWidget(new QLabel("活跃会员:"), 1, 0);
    activeMembersLabel = new QLabel("0");
    activeMembersLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; color: green; }");
    statsLayout->addWidget(activeMembersLabel, 1, 1);
    
    statsLayout->addWidget(new QLabel("过期会员:"), 2, 0);
    expiredMembersLabel = new QLabel("0");
    expiredMembersLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; color: red; }");
    statsLayout->addWidget(expiredMembersLabel, 2, 1);
    
    statsLayout->addWidget(new QLabel("会员等级分布:"), 3, 0, 1, 2);
    membersByLevelLabel = new QLabel("无数据");
    statsLayout->addWidget(membersByLevelLabel, 4, 0, 1, 2);
    
    // 课程统计
    statsLayout->addWidget(new QLabel("总课程数:"), 0, 2);
    totalCoursesLabel = new QLabel("0");
    totalCoursesLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    statsLayout->addWidget(totalCoursesLabel, 0, 3);
    
    statsLayout->addWidget(new QLabel("可预约课程:"), 1, 2);
    availableCoursesLabel = new QLabel("0");
    availableCoursesLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; color: green; }");
    statsLayout->addWidget(availableCoursesLabel, 1, 3);
    
    statsLayout->addWidget(new QLabel("总预约次数:"), 2, 2);
    totalBookingsLabel = new QLabel("0");
    totalBookingsLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    statsLayout->addWidget(totalBookingsLabel, 2, 3);
    
    statsLayout->addWidget(new QLabel("总签到次数:"), 3, 2);
    totalCheckInsLabel = new QLabel("0");
    totalCheckInsLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");
    statsLayout->addWidget(totalCheckInsLabel, 3, 3);
    
    mainLayout->addWidget(overviewGroup);
    
    // 数据图表 - 每日活动趋势
    QGroupBox *chartGroup = new QGroupBox("每日活动趋势");
    QVBoxLayout *chartLayout = new QVBoxLayout(chartGroup);
    
    dailyActivityChart = new QtCharts::QChartView();
    chartLayout->addWidget(dailyActivityChart);
    mainLayout->addWidget(chartGroup);
    
    // 按课程统计表格
    QHBoxLayout *tableLayout = new QHBoxLayout();
    
    // 预约统计表格
    QGroupBox *bookingsGroup = new QGroupBox("课程预约统计");
    QVBoxLayout *bookingsLayout = new QVBoxLayout(bookingsGroup);
    
    bookingsByCourseTable = new QTableWidget();
    bookingsByCourseTable->setColumnCount(2);
    bookingsByCourseTable->setHorizontalHeaderLabels({"课程名称", "预约次数"});
    bookingsByCourseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookingsByCourseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingsLayout->addWidget(bookingsByCourseTable);
    
    // 签到统计表格
    QGroupBox *checkInsGroup = new QGroupBox("课程签到统计");
    QVBoxLayout *checkInsLayout = new QVBoxLayout(checkInsGroup);
    
    checkInsByCourseTable = new QTableWidget();
    checkInsByCourseTable->setColumnCount(2);
    checkInsByCourseTable->setHorizontalHeaderLabels({"课程名称", "签到次数"});
    checkInsByCourseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    checkInsByCourseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    checkInsLayout->addWidget(checkInsByCourseTable);
    
    tableLayout->addWidget(bookingsGroup);
    tableLayout->addWidget(checkInsGroup);
    
    mainLayout->addLayout(tableLayout);
    
    // 连接信号槽
    connect(data, &GymData::dataChanged, this, &StatisticsTab::refresh);
    connect(updateBtn, &QPushButton::clicked, this, &StatisticsTab::updateChartRange);
    
    refresh();
}

void StatisticsTab::refresh()
{
    // 更新会员统计
    totalMembersLabel->setText(QString::number(data->getTotalMembers()));
    activeMembersLabel->setText(QString::number(data->getActiveMembers()));
    expiredMembersLabel->setText(QString::number(data->getExpiredMembers()));
    
    // 更新会员等级分布
    QMap<QString, int> membersByLevel = data->getMembersByLevel();
    QString levelText;
    for (const auto &level : membersByLevel.keys()) {
        levelText += QString("%1: %2人\n").arg(level).arg(membersByLevel[level]);
    }
    if (levelText.isEmpty()) {
        levelText = "无数据";
    } else {
        levelText.chop(1); // 移除最后一个换行符
    }
    membersByLevelLabel->setText(levelText);
    
    // 更新课程统计
    totalCoursesLabel->setText(QString::number(data->getTotalCourses()));
    availableCoursesLabel->setText(QString::number(data->getAvailableCourses()));
    totalBookingsLabel->setText(QString::number(data->getTotalBookings()));
    totalCheckInsLabel->setText(QString::number(data->getTotalCheckIns()));
    
    // 更新按课程统计表格
    QMap<QString, int> bookingsByCourse = data->getBookingsByCourse();
    bookingsByCourseTable->setRowCount(0);
    for (const auto &course : bookingsByCourse.keys()) {
        int row = bookingsByCourseTable->rowCount();
        bookingsByCourseTable->insertRow(row);
        bookingsByCourseTable->setItem(row, 0, new QTableWidgetItem(course));
        bookingsByCourseTable->setItem(row, 1, new QTableWidgetItem(QString::number(bookingsByCourse[course])));
    }
    
    QMap<QString, int> checkInsByCourse = data->getCheckInsByCourse();
    checkInsByCourseTable->setRowCount(0);
    for (const auto &course : checkInsByCourse.keys()) {
        int row = checkInsByCourseTable->rowCount();
        checkInsByCourseTable->insertRow(row);
        checkInsByCourseTable->setItem(row, 0, new QTableWidgetItem(course));
        checkInsByCourseTable->setItem(row, 1, new QTableWidgetItem(QString::number(checkInsByCourse[course])));
    }
    
    // 更新图表
    updateChartRange();
}

void StatisticsTab::updateChartRange()
{
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();
    
    // 获取每日活动数据
    QMap<QDate, int> dailyActivity = data->getDailyActivity(startDate, endDate);
    
    // 创建折线图
    QLineSeries *series = new QLineSeries();
    
    QDate currentDate = startDate;
    while (currentDate <= endDate) {
        *series << QPointF(currentDate.toJulianDay(), dailyActivity[currentDate]);
        currentDate = currentDate.addDays(1);
    }
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("每日活动趋势");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // 设置X轴
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setTickCount(7);
    axisX->setFormat("MM-dd");
    axisX->setTitleText("日期");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    // 设置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("活动次数");
    axisY->setTickCount(5);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    // 设置图表视图
    dailyActivityChart->setChart(chart);
    dailyActivityChart->setRenderHint(QPainter::Antialiasing);
}
