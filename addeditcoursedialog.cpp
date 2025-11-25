#include "AddEditCourseDialog.h"
#include <QMessageBox>

AddEditCourseDialog::AddEditCourseDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode) {
    setWindowTitle(QStringLiteral("添加课程"));
    setFixedSize(450, 350);
    initUI();
}

AddEditCourseDialog::AddEditCourseDialog(Mode mode, const Course& course, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_course(course) {
    setWindowTitle(QStringLiteral("编辑课程"));
    setFixedSize(450, 350);
    initUI();
    fillData();
    if (mode == EditMode) {
        idEdit->setReadOnly(true); // 编辑模式下ID不可修改
    }
}

void AddEditCourseDialog::initUI() {
    // 布局设置
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(30, 30, 30, 20);

    // 标签和输入组件
    idLabel = new QLabel(QStringLiteral("课程ID："), this);
    nameLabel = new QLabel(QStringLiteral("课程名称："), this);
    teacherLabel = new QLabel(QStringLiteral("授课老师："), this);
    timeLabel = new QLabel(QStringLiteral("上课时间："), this);
    maxNumLabel = new QLabel(QStringLiteral("最大人数："), this);

    idEdit = new QLineEdit(this);
    nameEdit = new QLineEdit(this);
    teacherEdit = new QLineEdit(this);
    timeEdit = new QLineEdit(this);
    maxNumSpin = new QSpinBox(this);

    // 设置占位符和验证器
    idEdit->setPlaceholderText(QStringLiteral("例如：C001（3-10位字母数字）"));
    idEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[A-Za-z0-9]{3,10}")), this));

    nameEdit->setPlaceholderText(QStringLiteral("2-20位中文或字母"));
    nameEdit->setAttribute(Qt::WA_InputMethodEnabled, true); // 启用中文输入法支持
    nameEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[一-龥A-Za-z0-9\\s]{2,20}")), this)); // 兼容Qt5.14的中文正则

    teacherEdit->setPlaceholderText(QStringLiteral("2-10位中文或字母"));
    teacherEdit->setAttribute(Qt::WA_InputMethodEnabled, true); // 启用中文输入法支持
    teacherEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[一-龥A-Za-z]{2,10}")), this)); // 兼容Qt5.14的中文正则

    timeEdit->setPlaceholderText(QStringLiteral("例如：周一19:00-21:00"));
    timeEdit->setAttribute(Qt::WA_InputMethodEnabled, true); // 启用中文输入法支持（时间可能包含中文）
    timeEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[一-龥A-Za-z0-9:\\-\\s]{5,30}")), this));

    // 最大人数范围：1-100
    maxNumSpin->setRange(1, 100);
    maxNumSpin->setValue(20); // 默认值

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    confirmBtn = new QPushButton(QStringLiteral("确认"), this);
    cancelBtn = new QPushButton(QStringLiteral("取消"), this);
    btnLayout->addStretch();
    btnLayout->addWidget(confirmBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addStretch();

    // 添加到网格布局
    gridLayout->addWidget(idLabel, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(idEdit, 0, 1);
    gridLayout->addWidget(nameLabel, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(nameEdit, 1, 1);
    gridLayout->addWidget(teacherLabel, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(teacherEdit, 2, 1);
    gridLayout->addWidget(timeLabel, 3, 0, Qt::AlignRight);
    gridLayout->addWidget(timeEdit, 3, 1);
    gridLayout->addWidget(maxNumLabel, 4, 0, Qt::AlignRight);
    gridLayout->addWidget(maxNumSpin, 4, 1);
    gridLayout->addLayout(btnLayout, 5, 0, 1, 2);

    // 连接信号槽
    connect(confirmBtn, &QPushButton::clicked, this, &AddEditCourseDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddEditCourseDialog::onCancelClicked);
}

void AddEditCourseDialog::fillData() {
    // 编辑模式下填充现有数据
    idEdit->setText(m_course.id());
    nameEdit->setText(m_course.name());
    teacherEdit->setText(m_course.teacher());
    timeEdit->setText(m_course.time());
    maxNumSpin->setValue(m_course.maxNum());
}

bool AddEditCourseDialog::validateInput() {
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString teacher = teacherEdit->text().trimmed();
    QString time = timeEdit->text().trimmed();

    // 验证ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("课程ID不能为空！"));
        idEdit->setFocus();
        return false;
    }

    // 验证课程名称
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("课程名称不能为空！"));
        nameEdit->setFocus();
        return false;
    }

    // 验证老师姓名
    if (teacher.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("授课老师不能为空！"));
        teacherEdit->setFocus();
        return false;
    }

    // 验证上课时间
    if (time.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("上课时间不能为空！"));
        timeEdit->setFocus();
        return false;
    }

    return true;
}

void AddEditCourseDialog::onConfirmClicked() {
    if (validateInput()) {
        // 构建课程对象
        m_course.setId(idEdit->text().trimmed());
        m_course.setName(nameEdit->text().trimmed());
        m_course.setTeacher(teacherEdit->text().trimmed());
        m_course.setTime(timeEdit->text().trimmed());
        m_course.setMaxNum(maxNumSpin->value());
        accept(); // 关闭对话框并返回QDialog::Accepted
    }
}

void AddEditCourseDialog::onCancelClicked() {
    reject(); // 关闭对话框并返回QDialog::Rejected
}

Course AddEditCourseDialog::getCourse() const {
    return m_course;
}
