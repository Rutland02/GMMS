#include "querytab.h"
#include "../core/gymdata.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>

QueryTab::QueryTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("最近操作记录:"));

    recordTable = new QTableWidget();
    recordTable->setColumnCount(4);
    recordTable->setHorizontalHeaderLabels({"时间","类型","会员姓名","课程名称"});
    recordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(recordTable);

    connect(data, &GymData::dataChanged, this, &QueryTab::refresh);
    refresh();
}

void QueryTab::refresh() {
    recordTable->setRowCount(0);
    for (const auto &r : data->getRecords()) {
        int row = recordTable->rowCount();
        recordTable->insertRow(row);
        recordTable->setItem(row, 0, new QTableWidgetItem(r.time.toString("MM-dd HH:mm")));
        recordTable->setItem(row, 1, new QTableWidgetItem(r.type));
        recordTable->setItem(row, 2, new QTableWidgetItem(r.memberName));
        recordTable->setItem(row, 3, new QTableWidgetItem(r.courseName));
    }
}
