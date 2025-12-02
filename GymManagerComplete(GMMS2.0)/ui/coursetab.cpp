#include "coursetab.h"
#include "../core/gymdata.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>

CourseTab::CourseTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QGroupBox *inputGroup = new QGroupBox("发布新课程", this);
    QFormLayout *form = new QFormLayout(inputGroup);

    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *coachEdit = new QLineEdit();
    QLineEdit *timeEdit = new QLineEdit();
    timeEdit->setPlaceholderText("例如: 周一 10:00");
    QSpinBox *maxSpin = new QSpinBox();
    maxSpin->setRange(1,100);
    maxSpin->setValue(10);
    QPushButton *addBtn = new QPushButton("添加课程");

    form->addRow("课程名称:", nameEdit);
    form->addRow("教练:", coachEdit);
    form->addRow("时间:", timeEdit);
    form->addRow("最大人数:", maxSpin);
    form->addRow(addBtn);

    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"课程ID","课程名称","教练","时间","人数(当前/最大)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(inputGroup,1);
    layout->addWidget(table,2);

    connect(addBtn, &QPushButton::clicked, this, [=](){
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "错误", "课程名称不能为空！");
            return;
        }
        QString id = QString("C%1").arg(data->getCourses().size() + 1, 2, 10, QChar('0'));
        Course c = {id, nameEdit->text(), coachEdit->text(), timeEdit->text(), maxSpin->value(), 0};
        if (!data->addCourse(c)) {
            QMessageBox::warning(this, "错误", "课程 ID 冲突！");
            return;
        }
        nameEdit->clear();
        coachEdit->clear();
        QMessageBox::information(this, "成功", "课程发布成功！");
    });

    connect(data, &GymData::dataChanged, this, &CourseTab::refresh);
    refresh();
}

void CourseTab::refresh() {
    table->setRowCount(0);
    const auto &courses = data->getCourses();
    for (const auto &c : courses) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(c.id));
        table->setItem(row, 1, new QTableWidgetItem(c.name));
        table->setItem(row, 2, new QTableWidgetItem(c.coach));
        table->setItem(row, 3, new QTableWidgetItem(c.timeStr));
        table->setItem(row, 4, new QTableWidgetItem(QString("%1 / %2").arg(c.currentBooked).arg(c.maxParticipants)));
    }
}
