#include "AddEditMemberDialog.h"
#include <QMessageBox>
#include <QDate>

AddEditMemberDialog::AddEditMemberDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode) {
    setWindowTitle(QStringLiteral("添加会员"));
    setFixedSize(400, 350); // 增加高度容纳新字段
    initUI();
}

AddEditMemberDialog::AddEditMemberDialog(Mode mode, const Member& member, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_member(member) {
    setWindowTitle(QStringLiteral("编辑会员"));
    setFixedSize(400, 350);
    initUI();
    fillData();
    if (mode == EditMode) {
        idEdit->setReadOnly(true);
    }
}

void AddEditMemberDialog::initUI() {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(30, 30, 30, 20);

    // 初始化标签和输入框（新增有效期）
    idLabel = new QLabel(QStringLiteral("会员ID："), this);
    nameLabel = new QLabel(QStringLiteral("姓名："), this);
    phoneLabel = new QLabel(QStringLiteral("联系电话："), this);
    joinDateLabel = new QLabel(QStringLiteral("加入日期："), this);
    expiryDateLabel = new QLabel(QStringLiteral("有效期至："), this); // 新增

    idEdit = new QLineEdit(this);
    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    joinDateEdit = new QLineEdit(this);
    expiryDateEdit = new QLineEdit(this); // 新增

    // 设置验证器（新增有效期验证）
    idEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[A-Za-z0-9]{3,10}")), this));
    nameEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[一-龥A-Za-z]{2,10}")), this));
    phoneEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[0-9]{11}")), this));
    joinDateEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("\\d{4}-\\d{2}-\\d{2}")), this));
    expiryDateEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("\\d{4}-\\d{2}-\\d{2}")), this)); // 新增

    // 占位符文本
    idEdit->setPlaceholderText(QStringLiteral("例如：M001"));
    nameEdit->setPlaceholderText(QStringLiteral("2-10位中文或字母"));
    phoneEdit->setPlaceholderText(QStringLiteral("11位手机号码"));
    joinDateEdit->setPlaceholderText(QStringLiteral("YYYY-MM-DD"));
    expiryDateEdit->setPlaceholderText(QStringLiteral("YYYY-MM-DD（需晚于加入日期）")); // 新增

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    confirmBtn = new QPushButton(QStringLiteral("确认"), this);
    cancelBtn = new QPushButton(QStringLiteral("取消"), this);
    btnLayout->addStretch();
    btnLayout->addWidget(confirmBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addStretch();

    // 添加到网格（新增有效期行）
    gridLayout->addWidget(idLabel, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(idEdit, 0, 1);
    gridLayout->addWidget(nameLabel, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(nameEdit, 1, 1);
    gridLayout->addWidget(phoneLabel, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(phoneEdit, 2, 1);
    gridLayout->addWidget(joinDateLabel, 3, 0, Qt::AlignRight);
    gridLayout->addWidget(joinDateEdit, 3, 1);
    gridLayout->addWidget(expiryDateLabel, 4, 0, Qt::AlignRight); // 新增
    gridLayout->addWidget(expiryDateEdit, 4, 1); // 新增
    gridLayout->addLayout(btnLayout, 5, 0, 1, 2);

    connect(confirmBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onCancelClicked);
}

void AddEditMemberDialog::fillData() {
    idEdit->setText(m_member.id());
    nameEdit->setText(m_member.name());
    phoneEdit->setText(m_member.phone());
    joinDateEdit->setText(m_member.joinDate());
    expiryDateEdit->setText(m_member.expiryDate()); // 新增
}

bool AddEditMemberDialog::validateInput() {
    // 原有验证逻辑...

    // 新增有效期验证
    QString expiryDate = expiryDateEdit->text().trimmed();
    if (expiryDate.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("有效期不能为空！"));
        return false;
    }

    QDate joinDate = QDate::fromString(joinDateEdit->text(), "yyyy-MM-dd");
    QDate expiry = QDate::fromString(expiryDate, "yyyy-MM-dd");
    if (!expiry.isValid()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("有效期格式无效！"));
        return false;
    }

    if (expiry <= joinDate) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("有效期必须晚于加入日期！"));
        return false;
    }

    return true;
}

void AddEditMemberDialog::onConfirmClicked() {
    if (validateInput()) {
        m_member.setId(idEdit->text().trimmed());
        m_member.setName(nameEdit->text().trimmed());
        m_member.setPhone(phoneEdit->text().trimmed());
        m_member.setJoinDate(joinDateEdit->text().trimmed());
        m_member.setExpiryDate(expiryDateEdit->text().trimmed()); // 新增
        accept();
    }
}

void AddEditMemberDialog::onCancelClicked() {
    reject();
}

Member AddEditMemberDialog::getMember() const {
    return m_member;
}
