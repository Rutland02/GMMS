#include "membertab.h"
#include "../core/gymdata.h"
#include "../model/member.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QVBoxLayout>

MemberTab::MemberTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout();

    // 添加新会员区域
    QGroupBox *addGroup = new QGroupBox("添加新会员", this);
    QFormLayout *addForm = new QFormLayout(addGroup);

    QLineEdit *idEdit = new QLineEdit();
    QLineEdit *nameEdit = new QLineEdit();
    QDateEdit *expiryEdit = new QDateEdit(QDate::currentDate().addYears(1));
    expiryEdit->setCalendarPopup(true);
    QComboBox *levelCombo = new QComboBox();
    levelCombo->addItems({"普通","白银","黄金","钻石"});
    QPushButton *addBtn = new QPushButton("注册会员");

    addForm->addRow("卡号:", idEdit);
    addForm->addRow("姓名:", nameEdit);
    addForm->addRow("有效期:", expiryEdit);
    addForm->addRow("等级:", levelCombo);
    addForm->addRow(addBtn);

    // 编辑会员区域
    QGroupBox *editGroup = new QGroupBox("编辑会员", this);
    QFormLayout *editForm = new QFormLayout(editGroup);

    QLineEdit *editIdEdit = new QLineEdit();
    QLineEdit *editNameEdit = new QLineEdit();
    QDateEdit *editExpiryEdit = new QDateEdit(QDate::currentDate().addYears(1));
    editExpiryEdit->setCalendarPopup(true);
    QComboBox *editLevelCombo = new QComboBox();
    editLevelCombo->addItems({"普通","白银","黄金","钻石"});
    QPushButton *editBtn = new QPushButton("修改会员");
    QPushButton *deleteBtn = new QPushButton("删除会员");
    
    // 积分调整功能
    QLineEdit *pointsEdit = new QLineEdit();
    pointsEdit->setPlaceholderText("输入积分数量");
    QPushButton *addPointsBtn = new QPushButton("增加积分");
    QPushButton *reducePointsBtn = new QPushButton("减少积分");

    editForm->addRow("卡号:", editIdEdit);
    editForm->addRow("姓名:", editNameEdit);
    editForm->addRow("有效期:", editExpiryEdit);
    editForm->addRow("等级:", editLevelCombo);
    editForm->addRow(editBtn);
    editForm->addRow(deleteBtn);
    editForm->addRow("积分调整:", pointsEdit);
    editForm->addRow(addPointsBtn);
    editForm->addRow(reducePointsBtn);

    leftLayout->addWidget(addGroup);
    leftLayout->addWidget(editGroup);

    // 会员列表
    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"卡号","姓名","有效期","等级","积分"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(leftLayout, 1);
    layout->addWidget(table, 2);

    // 添加会员
    connect(addBtn, &QPushButton::clicked, this, [=](){
        if (idEdit->text().isEmpty() || nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "错误", "卡号和姓名不能为空！");
            return;
        }
        Member m(idEdit->text(), nameEdit->text(), expiryEdit->date(), levelCombo->currentText());
        if (!data->addMember(m)) {
            QMessageBox::warning(this, "错误", "该卡号已存在！");
            return;
        }
        idEdit->clear();
        nameEdit->clear();
        QMessageBox::information(this, "成功", "会员注册成功！");
    });

    // 选择表格行时自动填充编辑表单
    connect(table, &QTableWidget::cellClicked, [=](int row, int col){
        Q_UNUSED(col);
        const auto &members = data->getMembers();
        if (row >= 0 && row < members.size()) {
            const Member &m = members[row];
            editIdEdit->setText(m.cardId());
            editNameEdit->setText(m.name());
            editExpiryEdit->setDate(m.expiryDate());
            int levelIndex = editLevelCombo->findText(m.level());
            if (levelIndex != -1) {
                editLevelCombo->setCurrentIndex(levelIndex);
            }
        }
    });

    // 编辑会员
    connect(editBtn, &QPushButton::clicked, [=](){
        int selectedRow = table->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(this, "错误", "请先选择要编辑的会员！");
            return;
        }
        if (editIdEdit->text().isEmpty() || editNameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "错误", "卡号和姓名不能为空！");
            return;
        }
        // 获取原会员的积分值
        const auto &members = data->getMembers();
        int points = 0;
        if (selectedRow >= 0 && selectedRow < members.size()) {
            points = members[selectedRow].points();
        }
        Member m(editIdEdit->text(), editNameEdit->text(), editExpiryEdit->date(), editLevelCombo->currentText(), points);
        if (!data->editMember(selectedRow, m)) {
            QMessageBox::warning(this, "错误", "该卡号已存在或编辑失败！");
            return;
        }
        QMessageBox::information(this, "成功", "会员信息修改成功！");
    });

    // 删除会员
    connect(deleteBtn, &QPushButton::clicked, [=](){
        int selectedRow = table->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(this, "错误", "请先选择要删除的会员！");
            return;
        }
        if (QMessageBox::question(this, "确认", "确定要删除选中的会员吗？") == QMessageBox::Yes) {
            if (!data->deleteMember(selectedRow)) {
                QMessageBox::warning(this, "错误", "删除会员失败！");
                return;
            }
            QMessageBox::information(this, "成功", "会员删除成功！");
        }
    });
    
    // 增加积分
    connect(addPointsBtn, &QPushButton::clicked, [=](){
        int selectedRow = table->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(this, "错误", "请先选择要调整积分的会员！");
            return;
        }
        bool ok;
        int points = pointsEdit->text().toInt(&ok);
        if (!ok || points <= 0) {
            QMessageBox::warning(this, "错误", "请输入有效的积分数量！");
            return;
        }
        
        auto &members = data->getMembers();
        if (selectedRow >= 0 && selectedRow < members.size()) {
            Member member = members[selectedRow];
            member.addPoints(points);
            if (data->editMember(selectedRow, member)) {
                QMessageBox::information(this, "成功", "积分增加成功！");
                pointsEdit->clear();
            } else {
                QMessageBox::warning(this, "错误", "积分增加失败！");
            }
        }
    });
    
    // 减少积分
    connect(reducePointsBtn, &QPushButton::clicked, [=](){
        int selectedRow = table->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(this, "错误", "请先选择要调整积分的会员！");
            return;
        }
        bool ok;
        int points = pointsEdit->text().toInt(&ok);
        if (!ok || points <= 0) {
            QMessageBox::warning(this, "错误", "请输入有效的积分数量！");
            return;
        }
        
        auto &members = data->getMembers();
        if (selectedRow >= 0 && selectedRow < members.size()) {
            Member member = members[selectedRow];
            if (member.points() < points) {
                QMessageBox::warning(this, "错误", "积分不足，无法减少！");
                return;
            }
            member.reducePoints(points);
            if (data->editMember(selectedRow, member)) {
                QMessageBox::information(this, "成功", "积分减少成功！");
                pointsEdit->clear();
            } else {
                QMessageBox::warning(this, "错误", "积分减少失败！");
            }
        }
    });

    connect(data, &GymData::dataChanged, this, &MemberTab::refresh);
    refresh();
}

void MemberTab::refresh() {
    table->setRowCount(0);
    const auto &members = data->getMembers();
    for (const auto &m : members) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(m.cardId()));
        table->setItem(row, 1, new QTableWidgetItem(m.name()));
        table->setItem(row, 2, new QTableWidgetItem(m.expiryDate().toString("yyyy-MM-dd")));
        table->setItem(row, 3, new QTableWidgetItem(m.level()));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(m.points())));
    }
}
