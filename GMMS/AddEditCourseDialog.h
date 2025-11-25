#ifndef ADDEDITCOURSEDIALOG_H
#define ADDEDITCOURSEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Course.h"

class AddEditCourseDialog : public QDialog
{
    Q_OBJECT

public:
    // 模式：添加（0）/ 编辑（1）
    enum Mode { AddMode = 0, EditMode = 1 };

    // 构造函数：添加模式（无需初始课程）
    explicit AddEditCourseDialog(Mode mode, QWidget *parent = nullptr);
    // 构造函数：编辑模式（需要传入初始课程数据）
    AddEditCourseDialog(Mode mode, const Course &course, QWidget *parent = nullptr);

    // 获取用户输入的课程数据
    Course getCourseData() const;

private slots:
    // 确认按钮事件
    void onConfirmClicked();
    // 取消按钮事件
    void onCancelClicked();

private:
    Mode m_mode;              // 对话框模式
    Course m_course;          // 存储课程数据

    // UI组件
    QLabel *idLabel;
    QLineEdit *idEdit;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *teacherLabel;
    QLineEdit *teacherEdit;
    QLabel *timeLabel;
    QLineEdit *timeEdit;
    QLabel *maxNumLabel;
    QLineEdit *maxNumEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    // 初始化UI
    void initUI();
    // 填充编辑模式的初始数据
    void fillCourseData(const Course &course);
    // 校验输入数据是否合法
    bool validateInput();
};

#endif // ADDEDITCOURSEDIALOG_H
