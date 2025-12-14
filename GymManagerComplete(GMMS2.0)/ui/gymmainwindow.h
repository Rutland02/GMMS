#ifndef GYMMAINWINDOW_H
#define GYMMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

class GymData;
class MemberTab;
class CourseTab;
class BookingTab;
class QueryTab;
class StatisticsTab;

class GymMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GymMainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void applyStyles();

    GymData *data;
    QTabWidget *tabs;

    MemberTab *memberTab;
    CourseTab *courseTab;
    BookingTab *bookingTab;
    QueryTab *queryTab;
    StatisticsTab *statisticsTab;
};

#endif // GYMMAINWINDOW_H
