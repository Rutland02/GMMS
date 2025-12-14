#ifndef STATISTICSTAB_H
#define STATISTICSTAB_H

#include <QWidget>

class GymData;
class QLabel;
class QGridLayout;
class QDateEdit;
class QPushButton;
class QTableWidget;
namespace QtCharts { class QChartView; };

class StatisticsTab : public QWidget {
    Q_OBJECT
public:
    explicit StatisticsTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private slots:
    void updateChartRange();

private:
    GymData *data;
    QGridLayout *statsLayout;
    
    // 会员统计标签
    QLabel *totalMembersLabel;
    QLabel *activeMembersLabel;
    QLabel *expiredMembersLabel;
    QLabel *membersByLevelLabel;
    
    // 课程统计标签
    QLabel *totalCoursesLabel;
    QLabel *availableCoursesLabel;
    QLabel *totalBookingsLabel;
    QLabel *totalCheckInsLabel;
    
    // 日期范围选择
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QPushButton *updateBtn;
    
    // 数据表格
    QTableWidget *bookingsByCourseTable;
    QTableWidget *checkInsByCourseTable;
    
    // 图表
    QtCharts::QChartView *dailyActivityChart;
};

#endif // STATISTICSTAB_H