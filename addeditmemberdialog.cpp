#include "AddEditMemberDialog.h"
#include <QMessageBox>
#include <QDate>

AddEditMemberDialog::AddEditMemberDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode) {
    setWindowTitle(QStringLiteral("添加会员"));
    setFixedSize(400, 300);
    initUI();
}

AddEditMemberDialog::AddEditMemberDialog(Mode mode, const Member& member, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_member(member) {
    setWindowTitle(QStringLiteral("编辑会员"));
    setFixedSize(400, 300);
    initUI();
    fillData();
    if (mode == EditMode) {
        idEdit->setReadOnly(true); // 编辑模式下ID不可修改
    }
}

void AddEditMemberDialog::initUI() {
    // 布局设置
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(30, 30, 30, 20);

    // 标签和输入框
    idLabel = new QLabel(QStringLiteral("会员ID："), this);
    nameLabel = new QLabel(QStringLiteral("姓名："), this);
    phoneLabel = new QLabel(QStringLiteral("联系电话："), this);
    joinDateLabel = new QLabel(QStringLiteral("加入日期："), this);

    idEdit = new QLineEdit(this);
    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    joinDateEdit = new QLineEdit(this);

    // 设置占位符和验证器
    idEdit->setPlaceholderText(QStringLiteral("例如：M001（3-10位字母数字）"));
    idEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[A-Za-z0-9]{3,10}")), this));

    nameEdit->setPlaceholderText(QStringLiteral("2-10位中文或字母"));
    nameEdit->setAttribute(Qt::WA_InputMethodEnabled, true); // 启用中文输入法支持
    nameEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[一-龥A-Za-z]{2,10}")), this)); // 兼容Qt5.14的中文正则

    phoneEdit->setPlaceholderText(QStringLiteral("11位手机号码"));
    phoneEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[0-9]{11}")), this));

    joinDateEdit->setPlaceholderText(QStringLiteral("格式：YYYY-MM-DD（例如：2025-11-25）"));
    joinDateEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("\\d{4}-\\d{2}-\\d{2}")), this));

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
    gridLayout->addWidget(phoneLabel, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(phoneEdit, 2, 1);
    gridLayout->addWidget(joinDateLabel, 3, 0, Qt::AlignRight);
    gridLayout->addWidget(joinDateEdit, 3, 1);
    gridLayout->addLayout(btnLayout, 4, 0, 1, 2);

    // 连接信号槽
    connect(confirmBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onCancelClicked);
}

void AddEditMemberDialog::fillData() {
    // 编辑模式下填充现有数据
    idEdit->setText(m_member.id());
    nameEdit->setText(m_member.name());
    phoneEdit->setText(m_member.phone());
    joinDateEdit->setText(m_member.joinDate());
}

bool AddEditMemberDialog::validateInput() {
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString joinDate = joinDateEdit->text().trimmed();

    // 验证ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("会员ID不能为空！"));
        idEdit->setFocus();
        return false;
    }

    // 验证姓名
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("姓名不能为空！"));
        nameEdit->setFocus();
        return false;
    }

    // 验证手机号
    if (phone.isEmpty() || phone.length() != 11) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("请输入11位有效手机号码！"));
        phoneEdit->setFocus();
        return false;
    }

    // 验证日期格式
    if (joinDate.isEmpty() || joinDate.length() != 10 || joinDate.count(QChar('-')) != 2) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("日期格式错误！请按YYYY-MM-DD填写。"));
        joinDateEdit->setFocus();
        return false;
    }

    // 验证日期有效性（可选）
    QStringList dateParts = joinDate.split('-');
    int year = dateParts[0].toInt();
    int month = dateParts[1].toInt();
    int day = dateParts[2].toInt();
    QDate date(year, month, day);
    if (!date.isValid()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("日期无效！请填写正确的日期。"));
        joinDateEdit->setFocus();
        return false;
    }

    return true;
}

void AddEditMemberDialog::onConfirmClicked() {
    if (validateInput()) {
        // 构建会员对象
        m_member.setId(idEdit->text().trimmed());
        m_member.setName(nameEdit->text().trimmed());
        m_member.setPhone(phoneEdit->text().trimmed());
        m_member.setJoinDate(joinDateEdit->text().trimmed());
        accept(); // 关闭对话框并返回QDialog::Accepted
    }
}

void AddEditMemberDialog::onCancelClicked() {
    reject(); // 关闭对话框并返回QDialog::Rejected
}

Member AddEditMemberDialog::getMember() const {
    return m_member;
}
