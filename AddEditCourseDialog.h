#ifndef ADDEDITCOURSEDIALOG_H
#define ADDEDITCOURSEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QSpinBox>
#include "Course.h"

class AddEditCourseDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { AddMode, EditMode };

    // 构造函数（添加模式）
    AddEditCourseDialog(Mode mode, QWidget *parent = nullptr);
    // 构造函数（编辑模式）
    AddEditCourseDialog(Mode mode, const Course& course, QWidget *parent = nullptr);

    // 获取输入的课程数据
    Course getCourse() const;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    void initUI();
    void fillData();
    bool validateInput();

    Mode m_mode;
    Course m_course;

    // UI组件
    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *teacherLabel;
    QLabel *timeLabel;
    QLabel *maxNumLabel;

    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *teacherEdit;
    QLineEdit *timeEdit;
    QSpinBox *maxNumSpin;

    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
};

#endif // ADDEDITCOURSEDIALOG_H
