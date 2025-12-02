#ifndef GYMMAINWINDOW_H
#define GYMMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

class GymData;
class MemberTab;
class CourseTab;
class BookingTab;
class QueryTab;

class GymMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GymMainWindow(QWidget *parent = nullptr);

private:
    void applyStyles();

    GymData *data;
    QTabWidget *tabs;

    MemberTab *memberTab;
    CourseTab *courseTab;
    BookingTab *bookingTab;
    QueryTab *queryTab;
};

#endif // GYMMAINWINDOW_H
