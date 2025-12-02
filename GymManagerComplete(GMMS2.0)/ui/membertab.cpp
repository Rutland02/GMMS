#include "membertab.h"
#include "../core/gymdata.h"
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

MemberTab::MemberTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QGroupBox *inputGroup = new QGroupBox("添加新会员", this);
    QFormLayout *form = new QFormLayout(inputGroup);

    QLineEdit *idEdit = new QLineEdit();
    QLineEdit *nameEdit = new QLineEdit();
    QDateEdit *expiryEdit = new QDateEdit(QDate::currentDate().addYears(1));
    expiryEdit->setCalendarPopup(true);
    QComboBox *levelCombo = new QComboBox();
    levelCombo->addItems({"普通","白银","黄金","钻石"});
    QPushButton *addBtn = new QPushButton("注册会员");

    form->addRow("卡号:", idEdit);
    form->addRow("姓名:", nameEdit);
    form->addRow("有效期:", expiryEdit);
    form->addRow("等级:", levelCombo);
    form->addRow(addBtn);

    table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"卡号","姓名","有效期","等级"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(inputGroup,1);
    layout->addWidget(table,2);

    connect(addBtn, &QPushButton::clicked, this, [=](){
        if (idEdit->text().isEmpty() || nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "错误", "卡号和姓名不能为空！");
            return;
        }
        Member m = {idEdit->text(), nameEdit->text(), expiryEdit->date(), levelCombo->currentText()};
        if (!data->addMember(m)) {
            QMessageBox::warning(this, "错误", "该卡号已存在！");
            return;
        }
        idEdit->clear();
        nameEdit->clear();
        QMessageBox::information(this, "成功", "会员注册成功！");
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
        table->setItem(row, 0, new QTableWidgetItem(m.cardId));
        table->setItem(row, 1, new QTableWidgetItem(m.name));
        table->setItem(row, 2, new QTableWidgetItem(m.expiryDate.toString("yyyy-MM-dd")));
        table->setItem(row, 3, new QTableWidgetItem(m.level));
    }
}
