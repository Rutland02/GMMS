#include "AddEditCourseDialog.h"
#include <QMessageBox>
#include <QRegExpValidator>

AddEditCourseDialog::AddEditCourseDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode)
{
    initUI();
    setWindowTitle("添加课程");
}

AddEditCourseDialog::AddEditCourseDialog(Mode mode, const Course &course, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_course(course)
{
    initUI();
    setWindowTitle("编辑课程");
    fillCourseData(course); // 填充初始数据
    if (mode == EditMode) {
        idEdit->setEnabled(false); // 编辑模式下ID不可修改
    }
}

// 获取用户输入的课程数据
Course AddEditCourseDialog::getCourseData() const
{
    Course course;
    course.setId(idEdit->text().trimmed());
    course.setName(nameEdit->text().trimmed());
    course.setTeacher(teacherEdit->text().trimmed());
    course.setTime(timeEdit->text().trimmed());
    course.setMaxNum(maxNumEdit->text().trimmed().toInt());
    return course;
}

// 初始化UI
void AddEditCourseDialog::initUI()
{
    // 设置对话框大小和固定尺寸
    setFixedSize(380, 300);

    // 创建标签和输入框
    idLabel = new QLabel("课程ID：");
    idEdit = new QLineEdit();
    idEdit->setPlaceholderText("请输入唯一ID（如C001）");
    idEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{3,10}"), this));

    nameLabel = new QLabel("课程名称：");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("请输入课程名称");
    nameEdit->setValidator(new QRegExpValidator(QRegExp("[\\u4e00-\\u9fa5A-Za-z0-9]{2,20}"), this));

    teacherLabel = new QLabel("授课老师：");
    teacherEdit = new QLineEdit();
    teacherEdit->setPlaceholderText("请输入老师姓名");
    teacherEdit->setValidator(new QRegExpValidator(QRegExp("[\\u4e00-\\u9fa5A-Za-z]{2,10}"), this));

    timeLabel = new QLabel("上课时间：");
    timeEdit = new QLineEdit();
    timeEdit->setPlaceholderText("格式：周X XX:XX-XX:XX（如周一 19:00-20:30）");

    maxNumLabel = new QLabel("最大人数：");
    maxNumEdit = new QLineEdit();
    maxNumEdit->setPlaceholderText("请输入正整数（如20）");
    maxNumEdit->setValidator(new QRegExpValidator(QRegExp("[1-9]\\d*"), this)); // 正整数

    // 创建按钮
    confirmBtn = new QPushButton("确认");
    cancelBtn = new QPushButton("取消");
    // 绑定按钮事件
    connect(confirmBtn, &QPushButton::clicked, this, &AddEditCourseDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddEditCourseDialog::onCancelClicked);

    // 布局管理（垂直布局：输入区域 + 按钮区域）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // 输入区域（网格布局：5行2列）
    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(idLabel, 0, 0, Qt::AlignRight);
    inputLayout->addWidget(idEdit, 0, 1);
    inputLayout->addWidget(nameLabel, 1, 0, Qt::AlignRight);
    inputLayout->addWidget(nameEdit, 1, 1);
    inputLayout->addWidget(teacherLabel, 2, 0, Qt::AlignRight);
    inputLayout->addWidget(teacherEdit, 2, 1);
    inputLayout->addWidget(timeLabel, 3, 0, Qt::AlignRight);
    inputLayout->addWidget(timeEdit, 3, 1);
    inputLayout->addWidget(maxNumLabel, 4, 0, Qt::AlignRight);
    inputLayout->addWidget(maxNumEdit, 4, 1);
    // 设置输入区域间距
    inputLayout->setSpacing(15);
    inputLayout->setContentsMargins(20, 20, 20, 20);

    // 按钮区域（水平布局）
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch(); // 左侧拉伸（按钮靠右）
    btnLayout->addWidget(confirmBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addStretch(); // 右侧拉伸
    btnLayout->setContentsMargins(20, 0, 20, 20);

    // 添加到主布局
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

// 填充编辑模式的初始数据
void AddEditCourseDialog::fillCourseData(const Course &course)
{
    idEdit->setText(course.id());
    nameEdit->setText(course.name());
    teacherEdit->setText(course.teacher());
    timeEdit->setText(course.time());
    maxNumEdit->setText(QString::number(course.maxNum()));
}

// 校验输入数据
bool AddEditCourseDialog::validateInput()
{
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString teacher = teacherEdit->text().trimmed();
    QString time = timeEdit->text().trimmed();
    QString maxNumStr = maxNumEdit->text().trimmed();

    // 校验ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "课程ID不能为空！");
        idEdit->setFocus();
        return false;
    }
    // 校验课程名称
    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "课程名称不能为空！");
        nameEdit->setFocus();
        return false;
    }
    // 校验老师姓名
    if (teacher.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "老师姓名不能为空！");
        teacherEdit->setFocus();
        return false;
    }
    // 校验上课时间（简单校验：包含"周"和"-"）
    if (time.isEmpty() || !time.contains("周") || !time.contains("-")) {
        QMessageBox::warning(this, "输入错误", "时间格式错误！请按「周X XX:XX-XX:XX」输入（如周一 19:00-20:30）");
        timeEdit->setFocus();
        return false;
    }
    // 校验最大人数
    if (maxNumStr.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "最大人数不能为空！");
        maxNumEdit->setFocus();
        return false;
    }
    int maxNum = maxNumStr.toInt();
    if (maxNum <= 0) {
        QMessageBox::warning(this, "输入错误", "最大人数必须为正整数！");
        maxNumEdit->setFocus();
        return false;
    }

    return true;
}

// 确认按钮事件
void AddEditCourseDialog::onConfirmClicked()
{
    if (validateInput()) {
        accept(); // 关闭对话框并返回Accepted状态
    }
}

// 取消按钮事件
void AddEditCourseDialog::onCancelClicked()
{
    reject(); // 关闭对话框并返回Rejected状态
}
