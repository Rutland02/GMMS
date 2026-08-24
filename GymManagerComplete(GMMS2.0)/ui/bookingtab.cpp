#include "bookingtab.h"
#include "../core/gymdata.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>

BookingTab::BookingTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QGroupBox *opGroup = new QGroupBox("操作台", this);
    QHBoxLayout *opLayout = new QHBoxLayout(opGroup);

    memberCombo = new QComboBox();
    courseCombo = new QComboBox();
    QPushButton *bookBtn = new QPushButton("🗓️ 预约课程");
    QPushButton *checkBtn = new QPushButton("✅ 现场签到");

    opLayout->addWidget(new QLabel("选择会员:"));
    opLayout->addWidget(memberCombo, 1);
    opLayout->addWidget(new QLabel("选择课程:"));
    opLayout->addWidget(courseCombo, 1);
    opLayout->addWidget(bookBtn);
    opLayout->addWidget(checkBtn);

    layout->addWidget(opGroup);

    recordTable = new QTableWidget();
    recordTable->setColumnCount(4);
    recordTable->setHorizontalHeaderLabels({"时间", "类型", "会员姓名", "课程名称"});
    recordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(recordTable);

    connect(bookBtn, &QPushButton::clicked, this, [=]() { handleBooking(true); });
    connect(checkBtn, &QPushButton::clicked, this, [=]() { handleBooking(false); });

    connect(data, &GymData::dataChanged, this, &BookingTab::refresh);
    refresh();
}

void BookingTab::handleBooking(bool isBooking)
{
    const QString memberId = memberCombo->currentData().toString();
    const QString courseId = courseCombo->currentData().toString();
    QString err;
    if (memberId.isEmpty() || courseId.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择会员和课程");
        return;
    }

    bool ok = isBooking ? data->bookCourse(memberId, courseId, err) : data->checkIn(memberId, courseId, err);
    if (!ok) {
        QMessageBox::warning(this, "失败", err);
        return;
    }
    QMessageBox::information(this, "成功", isBooking ? "预约成功" : "签到成功");
}

void BookingTab::refresh()
{
    memberCombo->clear();
    for (const auto &m : data->getMembers()) {
        memberCombo->addItem(QString("%1 - %2 (%3)").arg(m.cardId()).arg(m.name()).arg(m.level()), m.cardId());
    }
    courseCombo->clear();
    for (const auto &c : data->getCourses()) {
        if (!c.isExpired()) { // 只显示未过期的课程
            // 添加课程状态信息
            QString status;
            if (c.isFull()) {
                status = "[已满]";
            } else {
                status = "[可预约]";
            }
            courseCombo->addItem(QString("%1 - %2 (%3) %4").arg(c.name()).arg(c.coach()).arg(c.timeStr()).arg(status), c.id());
        }
    }

    // fill records table
    recordTable->setRowCount(0);
    for (const auto &r : data->getCheckIns()) {
        int row = recordTable->rowCount();
        recordTable->insertRow(row);
        recordTable->setItem(row, 0, new QTableWidgetItem(r.time().toString("MM-dd HH:mm")));
        recordTable->setItem(row, 1, new QTableWidgetItem(r.type()));
        recordTable->setItem(row, 2, new QTableWidgetItem(r.memberName()));
        recordTable->setItem(row, 3, new QTableWidgetItem(r.courseName()));
    }
}
