#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "DataManager.h"
#include "AddEditMemberDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 会员管理
    void onAddMemberClicked();
    void onEditMemberClicked();
    void onDeleteMemberClicked();
    void onSearchMemberClicked();

    // 签到与预约
    void onCheckInClicked();
    void onReserveCourseClicked();
    void onCancelReservationClicked();

private:
    void initUI();
    void initMemberTab();
    void initCourseTab();
    void initCheckInTab();

    void refreshMemberTable();
    void refreshCourseTable();
    void refreshCheckInTable();
    void refreshReservationTable();

    // UI组件
    QTabWidget *tabWidget;
    QWidget *memberTab;
    QWidget *courseTab;
    QWidget *checkInTab;

    // 会员管理组件
    QTableWidget *memberTable;
    QLineEdit *memberSearchEdit;
    QPushButton *searchMemberBtn;
    QPushButton *addMemberBtn;
    QPushButton *editMemberBtn;
    QPushButton *deleteMemberBtn;

    // 课程管理组件
    QTableWidget *courseTable;
    QPushButton *addCourseBtn;
    QPushButton *editCourseBtn;
    QPushButton *deleteCourseBtn;

    // 签到与预约组件
    QTableWidget *checkInTable;
    QTableWidget *reservationTable;
    QPushButton *checkInBtn;
    QPushButton *reserveBtn;
    QPushButton *cancelReserveBtn;
    QLineEdit *checkInMemberIdEdit;
    QLineEdit *checkInCourseIdEdit;
    QLineEdit *reserveMemberIdEdit;
    QLineEdit *reserveCourseIdEdit;

    DataManager *dataManager;
};

#endif // MAINWINDOW_H
