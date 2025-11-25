#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

// 提前声明类（避免循环引用）
class DataManager;
class AddEditMemberDialog;
class AddEditCourseDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 会员相关按钮事件
    void onAddMemberClicked();
    void onEditMemberClicked();
    void onDeleteMemberClicked();
    // 课程相关按钮事件
    void onAddCourseClicked();
    void onEditCourseClicked();
    void onDeleteCourseClicked();
    // 刷新表格数据
    void refreshMemberTable();
    void refreshCourseTable();

private:
    // 核心组件
    QTabWidget *tabWidget;          // 标签页容器
    QTableWidget *memberTable;      // 会员表格
    QTableWidget *courseTable;      // 课程表格
    DataManager *dataManager;       // 数据管理类实例

    // 会员区域按钮
    QPushButton *addMemberBtn;
    QPushButton *editMemberBtn;
    QPushButton *deleteMemberBtn;
    // 课程区域按钮
    QPushButton *addCourseBtn;
    QPushButton *editCourseBtn;
    QPushButton *deleteCourseBtn;

    // 初始化UI
    void initUI();
    // 初始化会员表格
    void initMemberTable();
    // 初始化课程表格
    void initCourseTable();
};

#endif // MAINWINDOW_H
