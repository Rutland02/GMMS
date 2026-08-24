#include "querytab.h"
#include "../core/gymdata.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

namespace {
QString csvEscape(const QString &value)
{
    QString escaped = value;
    escaped.replace('"', "\"\"");
    if (escaped.contains(',') || escaped.contains('"') || escaped.contains('\n') || escaped.contains('\r')) {
        escaped = QString("\"%1\"").arg(escaped);
    }
    return escaped;
}
}

QueryTab::QueryTab(GymData *data, QWidget *parent)
    : QWidget(parent), data(data)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 查询条件区域
    QGroupBox *filterGroup = new QGroupBox("查询条件");
    QGridLayout *filterLayout = new QGridLayout(filterGroup);
    
    // 类型筛选
    filterLayout->addWidget(new QLabel("记录类型:"), 0, 0);
    typeCombo = new QComboBox();
    typeCombo->addItem("全部");
    typeCombo->addItem("预约");
    typeCombo->addItem("签到");
    filterLayout->addWidget(typeCombo, 0, 1);
    
    // 时间范围
    filterLayout->addWidget(new QLabel("时间范围:"), 0, 2);
    startDateEdit = new QDateTimeEdit();
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDateTime(QDateTime::currentDateTime().addDays(-7)); // 默认显示最近7天
    endDateEdit = new QDateTimeEdit();
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDateTime(QDateTime::currentDateTime());
    filterLayout->addWidget(startDateEdit, 0, 3);
    filterLayout->addWidget(new QLabel("至"), 0, 4);
    filterLayout->addWidget(endDateEdit, 0, 5);
    
    // 搜索框
    filterLayout->addWidget(new QLabel("搜索:"), 1, 0);
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("输入会员姓名或课程名称搜索");
    filterLayout->addWidget(searchEdit, 1, 1, 1, 3);
    
    // 查询按钮
    QPushButton *searchBtn = new QPushButton("🔍 查询");
    QPushButton *exportBtn = new QPushButton("📥 导出数据");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(searchBtn);
    btnLayout->addWidget(exportBtn);
    btnLayout->addStretch();
    filterLayout->addLayout(btnLayout, 1, 4, 1, 2);
    
    mainLayout->addWidget(filterGroup);

    // 记录表格
    recordTable = new QTableWidget();
    recordTable->setColumnCount(5);
    recordTable->setHorizontalHeaderLabels({"时间", "类型", "会员ID", "会员姓名", "课程名称"});
    recordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recordTable->setSortingEnabled(true); // 启用排序功能
    
    mainLayout->addWidget(recordTable);

    connect(data, &GymData::dataChanged, this, &QueryTab::refresh);
    connect(searchBtn, &QPushButton::clicked, this, &QueryTab::applyFilters);
    connect(exportBtn, &QPushButton::clicked, this, &QueryTab::exportData);
    connect(searchEdit, &QLineEdit::returnPressed, this, &QueryTab::applyFilters);
    
    refresh();
}

void QueryTab::refresh()
{
    // 保存当前筛选条件
    QString typeFilter = typeCombo->currentText();
    QDateTime startDate = startDateEdit->dateTime();
    QDateTime endDate = endDateEdit->dateTime();
    QString searchText = searchEdit->text();
    
    // 重新加载表格
    recordTable->setRowCount(0);
    for (const auto &r : data->getCheckIns()) {
        // 应用筛选条件
        bool matchType = (typeFilter == "全部") || (r.type() == typeFilter);
        bool matchDate = (r.time() >= startDate) && (r.time() <= endDate);
        bool matchSearch = searchText.isEmpty() || 
                          r.memberName().contains(searchText, Qt::CaseInsensitive) || 
                          r.courseName().contains(searchText, Qt::CaseInsensitive);
        
        if (matchType && matchDate && matchSearch) {
            int row = recordTable->rowCount();
            recordTable->insertRow(row);
            recordTable->setItem(row, 0, new QTableWidgetItem(r.time().toString("yyyy-MM-dd HH:mm")));
            recordTable->setItem(row, 1, new QTableWidgetItem(r.type()));
            recordTable->setItem(row, 2, new QTableWidgetItem(r.memberId()));
            recordTable->setItem(row, 3, new QTableWidgetItem(r.memberName()));
            recordTable->setItem(row, 4, new QTableWidgetItem(r.courseName()));
        }
    }
}

void QueryTab::applyFilters()
{
    refresh();
}

void QueryTab::exportData()
{
    QString filePath = QFileDialog::getSaveFileName(this, "导出数据", "", "CSV文件 (*.csv)");
    if (filePath.isEmpty()) return;
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件进行写入!");
        return;
    }
    
    QTextStream out(&file);
    out << QChar(0xFEFF); // 让 Excel 更容易正确识别中文编码
    // 写入表头
    out << "时间,类型,会员ID,会员姓名,课程名称\n";
    
    // 写入数据
    for (int row = 0; row < recordTable->rowCount(); ++row) {
        for (int col = 0; col < recordTable->columnCount(); ++col) {
            if (col > 0) out << ",";
            out << csvEscape(recordTable->item(row, col)->text());
        }
        out << "\n";
    }
    
    file.close();
    QMessageBox::information(this, "成功", "数据导出成功!");
}
