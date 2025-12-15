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

    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QGroupBox *inputGroup = new QGroupBox("发布新课程", leftWidget);
    QFormLayout *form = new QFormLayout(inputGroup);

    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *courseTypeEdit = new QLineEdit();
    QTextEdit *descriptionEdit = new QTextEdit();
    QLineEdit *coachEdit = new QLineEdit();
    QLineEdit *timeEdit = new QLineEdit();
    timeEdit->setPlaceholderText("例如: 周一 10:00");
    QDoubleSpinBox *priceSpin = new QDoubleSpinBox();
    priceSpin->setRange(0.0, 999.99);
    priceSpin->setDecimals(2);
    priceSpin->setValue(68.0);
    QSpinBox *maxSpin = new QSpinBox();
    maxSpin->setRange(1,100);
    maxSpin->setValue(10);
    QPushButton *addBtn = new QPushButton("添加课程");

    form->addRow("课程名称:", nameEdit);
    form->addRow("课程类型:", courseTypeEdit);
    form->addRow("课程描述:", descriptionEdit);
    form->addRow("教练:", coachEdit);
    form->addRow("时间:", timeEdit);
    form->addRow("价格:", priceSpin);
    form->addRow("最大人数:", maxSpin);
    form->addRow(addBtn);

    QGroupBox *editGroup = new QGroupBox("编辑课程", leftWidget);
    QVBoxLayout *editLayout = new QVBoxLayout(editGroup);
    QPushButton *editBtn = new QPushButton("编辑选中课程");
    QPushButton *deleteBtn = new QPushButton("删除选中课程");
    editLayout->addWidget(editBtn);
    editLayout->addWidget(deleteBtn);

    QGroupBox *searchGroup = new QGroupBox("搜索课程", leftWidget);
    QVBoxLayout *searchLayout = new QVBoxLayout(searchGroup);
    
    // 搜索选项
    QHBoxLayout *searchOptionsLayout = new QHBoxLayout();
    QComboBox *searchTypeCombo = new QComboBox();
    searchTypeCombo->addItem("按名称搜索");
    searchTypeCombo->addItem("按类型搜索");
    searchTypeCombo->addItem("按教练搜索");
    searchOptionsLayout->addWidget(searchTypeCombo);
    
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("输入搜索关键词...");
    searchOptionsLayout->addWidget(searchEdit);
    
    QPushButton *searchBtn = new QPushButton("搜索");
    searchOptionsLayout->addWidget(searchBtn);
    
    QPushButton *resetBtn = new QPushButton("重置");
    searchOptionsLayout->addWidget(resetBtn);
    
    searchLayout->addLayout(searchOptionsLayout);

    leftLayout->addWidget(inputGroup);
    leftLayout->addWidget(editGroup);
    leftLayout->addWidget(searchGroup);

    table = new QTableWidget();
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({"课程ID","课程名称","类型","教练","时间","价格","人数(当前/最大)","状态"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(leftWidget,1);
    layout->addWidget(table,2);

    connect(addBtn, &QPushButton::clicked, this, [=](){
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "错误", "课程名称不能为空！");
            return;
        }
        QString id = QString("C%1").arg(data->getCourses().size() + 1, 2, 10, QChar('0'));
        Course c(id, nameEdit->text(), courseTypeEdit->text(), descriptionEdit->toPlainText(), 
                 coachEdit->text(), timeEdit->text(), priceSpin->value(), maxSpin->value(), 0);
        if (!data->addCourse(c)) {
            QMessageBox::warning(this, "错误", "课程 ID 冲突！");
            return;
        }
        nameEdit->clear();
        courseTypeEdit->clear();
        descriptionEdit->clear();
        coachEdit->clear();
        timeEdit->clear();
        QMessageBox::information(this, "成功", "课程发布成功！");
    });

    connect(editBtn, &QPushButton::clicked, this, [=](){
        int selectedRow = table->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(this, "错误", "请先选择要编辑的课程！");
            return;
        }

        const auto &courses = data->getCourses();
        if (selectedRow >= courses.size()) {
            QMessageBox::warning(this, "错误", "无效的课程选择！");
            return;
        }

        const Course &course = courses[selectedRow];

        // 创建编辑对话框
        QDialog *editDialog = new QDialog(this);
        editDialog->setWindowTitle("编辑课程");
        QFormLayout *editForm = new QFormLayout(editDialog);

        QLineEdit *editName = new QLineEdit(course.name());
        QLineEdit *editCourseType = new QLineEdit(course.courseType());
        QTextEdit *editDescription = new QTextEdit(course.description());
        QLineEdit *editCoach = new QLineEdit(course.coach());
        QLineEdit *editTime = new QLineEdit(course.timeStr());
        QDoubleSpinBox *editPrice = new QDoubleSpinBox();
        editPrice->setRange(0.0, 999.99);
        editPrice->setDecimals(2);
        editPrice->setValue(course.price());
        QSpinBox *editMax = new QSpinBox();
        editMax->setRange(1, 100);
        editMax->setValue(course.maxParticipants());

        editForm->addRow("课程名称:", editName);
        editForm->addRow("课程类型:", editCourseType);
        editForm->addRow("课程描述:", editDescription);
        editForm->addRow("教练:", editCoach);
        editForm->addRow("时间:", editTime);
        editForm->addRow("价格:", editPrice);
        editForm->addRow("最大人数:", editMax);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, editDialog);
        editForm->addRow(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, [=](){
            if (editName->text().isEmpty()) {
                QMessageBox::warning(this, "错误", "课程名称不能为空！");
                return;
            }

            Course updatedCourse(course.id(), editName->text(), editCourseType->text(), 
                                editDescription->toPlainText(), editCoach->text(), 
                                editTime->text(), editPrice->value(), editMax->value(), 
                                course.currentBooked());

            if (data->editCourse(selectedRow, updatedCourse)) {
                QMessageBox::information(this, "成功", "课程编辑成功！");
                editDialog->accept();
            } else {
                QMessageBox::warning(this, "错误", "课程编辑失败！");
            }
        });

        connect(buttonBox, &QDialogButtonBox::rejected, editDialog, &QDialog::reject);

    editDialog->exec();
});

connect(deleteBtn, &QPushButton::clicked, this, [=](){
    int selectedRow = table->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "错误", "请先选择要删除的课程！");
        return;
    }

    const auto &courses = data->getCourses();
    if (selectedRow >= courses.size()) {
        QMessageBox::warning(this, "错误", "无效的课程选择！");
        return;
    }

    const Course &course = courses[selectedRow];
    if (QMessageBox::question(this, "确认删除", 
                             QString("确定要删除课程 '%1' 吗？\n该操作不可恢复！").arg(course.name()),
                             QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (data->deleteCourse(selectedRow)) {
        QMessageBox::information(this, "成功", "课程删除成功！");
    } else {
        QMessageBox::warning(this, "错误", "课程删除失败！");
    }
});

connect(searchBtn, &QPushButton::clicked, this, [=](){
    QString searchText = searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入搜索关键词！");
        return;
    }

    const auto &courses = data->getCourses();
    QVector<const Course*> filteredCourses;

    int searchType = searchTypeCombo->currentIndex();
    for (const auto &course : courses) {
        bool match = false;
        switch (searchType) {
            case 0: // 按名称搜索
                if (course.name().contains(searchText, Qt::CaseInsensitive))
                    match = true;
                break;
            case 1: // 按类型搜索
                if (course.courseType().contains(searchText, Qt::CaseInsensitive))
                    match = true;
                break;
            case 2: // 按教练搜索
                if (course.coach().contains(searchText, Qt::CaseInsensitive))
                    match = true;
                break;
        }
        if (match) {
            filteredCourses.push_back(&course);
        }
    }

    // 更新表格显示搜索结果
    table->setRowCount(0);
    for (const auto &course : filteredCourses) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(course->id()));
        table->setItem(row, 1, new QTableWidgetItem(course->name()));
        table->setItem(row, 2, new QTableWidgetItem(course->courseType()));
        table->setItem(row, 3, new QTableWidgetItem(course->coach()));
        table->setItem(row, 4, new QTableWidgetItem(course->timeStr()));
        table->setItem(row, 5, new QTableWidgetItem(QString("¥%1").arg(course->price())));
        table->setItem(row, 6, new QTableWidgetItem(QString("%1 / %2").arg(course->currentBooked()).arg(course->maxParticipants())));
        
        // 显示课程状态
        QTableWidgetItem *statusItem = new QTableWidgetItem();
        if (course->isFull()) {
            statusItem->setText("已满");
            statusItem->setTextColor(Qt::red);
        } else {
            statusItem->setText("可预约");
            statusItem->setTextColor(Qt::green);
        }
        table->setItem(row, 7, statusItem);
    }

    QMessageBox::information(this, "搜索结果", QString("找到 %1 门符合条件的课程！").arg(filteredCourses.size()));
});

connect(resetBtn, &QPushButton::clicked, this, [=](){
    searchEdit->clear();
    searchTypeCombo->setCurrentIndex(0);
    refresh(); // 重置为显示所有课程
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
        table->setItem(row, 0, new QTableWidgetItem(c.id()));
        table->setItem(row, 1, new QTableWidgetItem(c.name()));
        table->setItem(row, 2, new QTableWidgetItem(c.courseType()));
        table->setItem(row, 3, new QTableWidgetItem(c.coach()));
        table->setItem(row, 4, new QTableWidgetItem(c.timeStr()));
        table->setItem(row, 5, new QTableWidgetItem(QString("¥%1").arg(c.price())));
        table->setItem(row, 6, new QTableWidgetItem(QString("%1 / %2").arg(c.currentBooked()).arg(c.maxParticipants())));
        
        // 显示课程状态
        QTableWidgetItem *statusItem = new QTableWidgetItem();
        if (c.isFull()) {
            statusItem->setText("已满");
            statusItem->setTextColor(Qt::red);
        } else {
            statusItem->setText("可预约");
            statusItem->setTextColor(Qt::green);
        }
        table->setItem(row, 7, statusItem);
    }
}
