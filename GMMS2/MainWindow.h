#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "DataManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 会员相关槽函数
    void onAddMemberClicked();
    void onEditMemberClicked();
    void onDeleteMemberClicked();
    void refreshMemberTable();

    // 课程相关槽函数
    void onAddCourseClicked();
    void onEditCourseClicked();
    void onDeleteCourseClicked();
    void refreshCourseTable();

private:
    void initUI();
    void initMemberTab();
    void initCourseTab();

    // UI组件
    QTabWidget *tabWidget;
    QWidget *memberTab;
    QWidget *courseTab;

    // 会员表格和按钮
    QTableWidget *memberTable;
    QPushButton *addMemberBtn;
    QPushButton *editMemberBtn;
    QPushButton *deleteMemberBtn;

    // 课程表格和按钮
    QTableWidget *courseTable;
    QPushButton *addCourseBtn;
    QPushButton *editCourseBtn;
    QPushButton *deleteCourseBtn;

    // 数据管理器
    DataManager *dataManager;
};

#endif // MAINWINDOW_H
