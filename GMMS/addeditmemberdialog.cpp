#include "AddEditMemberDialog.h"
#include <QMessageBox>
#include <QRegExpValidator>

AddEditMemberDialog::AddEditMemberDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode)
{
    initUI();
    setWindowTitle("添加会员");
}

AddEditMemberDialog::AddEditMemberDialog(Mode mode, const Member &member, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_member(member)
{
    initUI();
    setWindowTitle("编辑会员");
    fillMemberData(member); // 填充初始数据
    if (mode == EditMode) {
        idEdit->setEnabled(false); // 编辑模式下ID不可修改
    }
}

// 获取用户输入的会员数据
Member AddEditMemberDialog::getMemberData() const
{
    Member member;
    member.setId(idEdit->text().trimmed());
    member.setName(nameEdit->text().trimmed());
    member.setPhone(phoneEdit->text().trimmed());
    member.setJoinDate(joinDateEdit->text().trimmed());
    return member;
}

// 初始化UI
void AddEditMemberDialog::initUI()
{
    // 设置对话框大小和固定尺寸
    setFixedSize(350, 250);

    // 创建标签和输入框
    idLabel = new QLabel("会员ID：");
    idEdit = new QLineEdit();
    idEdit->setPlaceholderText("请输入唯一ID（如M001）");
    // 限制ID格式：字母+数字（长度3-10）
    idEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{3,10}"), this));

    nameLabel = new QLabel("姓名：");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("请输入会员姓名");
    nameEdit->setValidator(new QRegExpValidator(QRegExp("[\\u4e00-\\u9fa5A-Za-z]{2,10}"), this)); // 中文+英文

    phoneLabel = new QLabel("联系电话：");
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("请输入11位手机号");
    phoneEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{11}"), this)); // 11位数字

    joinDateLabel = new QLabel("加入日期：");
    joinDateEdit = new QLineEdit();
    joinDateEdit->setPlaceholderText("格式：YYYY-MM-DD（如2025-11-25）");
    joinDateEdit->setValidator(new QRegExpValidator(QRegExp("\\d{4}-\\d{2}-\\d{2}"), this));

    // 创建按钮
    confirmBtn = new QPushButton("确认");
    cancelBtn = new QPushButton("取消");
    // 绑定按钮事件
    connect(confirmBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddEditMemberDialog::onCancelClicked);

    // 布局管理（垂直布局：输入区域 + 按钮区域）
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // 输入区域（网格布局：4行2列）
    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(idLabel, 0, 0, Qt::AlignRight);
    inputLayout->addWidget(idEdit, 0, 1);
    inputLayout->addWidget(nameLabel, 1, 0, Qt::AlignRight);
    inputLayout->addWidget(nameEdit, 1, 1);
    inputLayout->addWidget(phoneLabel, 2, 0, Qt::AlignRight);
    inputLayout->addWidget(phoneEdit, 2, 1);
    inputLayout->addWidget(joinDateLabel, 3, 0, Qt::AlignRight);
    inputLayout->addWidget(joinDateEdit, 3, 1);
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
void AddEditMemberDialog::fillMemberData(const Member &member)
{
    idEdit->setText(member.id());
    nameEdit->setText(member.name());
    phoneEdit->setText(member.phone());
    joinDateEdit->setText(member.joinDate());
}

// 校验输入数据
bool AddEditMemberDialog::validateInput()
{
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString joinDate = joinDateEdit->text().trimmed();

    // 校验ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "会员ID不能为空！");
        idEdit->setFocus();
        return false;
    }
    // 校验姓名
    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "姓名不能为空！");
        nameEdit->setFocus();
        return false;
    }
    // 校验手机号
    if (phone.isEmpty() || phone.length() != 11) {
        QMessageBox::warning(this, "输入错误", "请输入11位有效手机号！");
        phoneEdit->setFocus();
        return false;
    }
    // 校验日期格式（简单校验：长度10 + 包含2个'-'）
    if (joinDate.isEmpty() || joinDate.length() != 10 || joinDate.count('-') != 2) {
        QMessageBox::warning(this, "输入错误", "日期格式错误！请按YYYY-MM-DD输入（如2025-11-25）");
        joinDateEdit->setFocus();
        return false;
    }

    return true;
}

// 确认按钮事件
void AddEditMemberDialog::onConfirmClicked()
{
    if (validateInput()) {
        accept(); // 关闭对话框并返回Accepted状态
    }
}

// 取消按钮事件
void AddEditMemberDialog::onCancelClicked()
{
    reject(); // 关闭对话框并返回Rejected状态
}
