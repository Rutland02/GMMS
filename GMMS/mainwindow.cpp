#include "MainWindow.h"
#include "DataManager.h"
#include "AddEditMemberDialog.h"
#include "AddEditCourseDialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化数据管理类
    dataManager = new DataManager(this);
    // 初始化UI
    initUI();
    // 刷新初始数据
    refreshMemberTable();
    refreshCourseTable();
}

MainWindow::~MainWindow()
{
    // 自动释放子组件（Qt父对象机制）
}

// 初始化UI
void MainWindow::initUI()
{
    // 1. 创建标签页容器
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget); // 设置为中心组件

    // 2. 初始化会员标签页
    QWidget *memberWidget = new QWidget();
    QVBoxLayout *memberLayout = new QVBoxLayout(memberWidget);
    initMemberTable(); // 初始化会员表格
    // 会员按钮区域
    QWidget *memberBtnWidget = new QWidget();
    QHBoxLayout *memberBtnLayout = new QHBoxLayout(memberBtnWidget);
    addMemberBtn = new QPushButton(QStringLiteral("添加会员"));
    editMemberBtn = new QPushButton(QStringLiteral("编辑会员"));
    deleteMemberBtn = new QPushButton(QStringLiteral("删除会员"));
    // 绑定按钮事件
    connect(addMemberBtn, &QPushButton::clicked, this, &MainWindow::onAddMemberClicked);
    connect(editMemberBtn, &QPushButton::clicked, this, &MainWindow::onEditMemberClicked);
    connect(deleteMemberBtn, &QPushButton::clicked, this, &MainWindow::onDeleteMemberClicked);
    // 添加按钮到布局
    memberBtnLayout->addWidget(addMemberBtn);
    memberBtnLayout->addWidget(editMemberBtn);
    memberBtnLayout->addWidget(deleteMemberBtn);
    // 会员标签页布局：按钮区域 + 表格
    memberLayout->addWidget(memberBtnWidget);
    memberLayout->addWidget(memberTable);
    tabWidget->addTab(memberWidget, QStringLiteral("会员管理"));

    // 3. 初始化课程标签页
    QWidget *courseWidget = new QWidget();
    QVBoxLayout *courseLayout = new QVBoxLayout(courseWidget);
    initCourseTable(); // 初始化课程表格
    // 课程按钮区域
    QWidget *courseBtnWidget = new QWidget();
    QHBoxLayout *courseBtnLayout = new QHBoxLayout(courseBtnWidget);
    addCourseBtn = new QPushButton(QStringLiteral("添加课程"));
    editCourseBtn = new QPushButton(QStringLiteral("编辑课程"));
    deleteCourseBtn = new QPushButton(QStringLiteral("删除课程"));
    // 绑定按钮事件
    connect(addCourseBtn, &QPushButton::clicked, this, &MainWindow::onAddCourseClicked);
    connect(editCourseBtn, &QPushButton::clicked, this, &MainWindow::onEditCourseClicked);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &MainWindow::onDeleteCourseClicked);
    // 添加按钮到布局
    courseBtnLayout->addWidget(addCourseBtn);
    courseBtnLayout->addWidget(editCourseBtn);
    courseBtnLayout->addWidget(deleteCourseBtn);
    // 课程标签页布局：按钮区域 + 表格
    courseLayout->addWidget(courseBtnWidget);
    courseLayout->addWidget(courseTable);
    tabWidget->addTab(courseWidget, QStringLiteral("课程管理"));
}

// 初始化会员表格
void MainWindow::initMemberTable()
{
    memberTable = new QTableWidget();
    // 设置列数和列名
    QStringList headers;
    headers << QStringLiteral("会员ID") << QStringLiteral("姓名") << QStringLiteral("联系电话") << QStringLiteral("加入日期");
    memberTable->setColumnCount(4);
    memberTable->setHorizontalHeaderLabels(headers);
    // 表格属性：禁止编辑、选中整行、自适应列宽
    memberTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memberTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    memberTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// 初始化课程表格
void MainWindow::initCourseTable()
{
    courseTable = new QTableWidget();
    // 设置列数和列名
    QStringList headers;
    headers << QStringLiteral("课程ID") << QStringLiteral("课程名称") << QStringLiteral("授课老师") << QStringLiteral("上课时间") << QStringLiteral("最大人数");
    courseTable->setColumnCount(5);
    courseTable->setHorizontalHeaderLabels(headers);
    // 表格属性：禁止编辑、选中整行、自适应列宽
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// 刷新会员表格
void MainWindow::refreshMemberTable()
{
    // 清空表格（保留列名）
    memberTable->setRowCount(0);
    // 获取所有会员数据
    QList<Member> members = dataManager->getAllMembers();
    // 填充表格
    for (int i = 0; i < members.size(); ++i) {
        Member member = members[i];
        memberTable->insertRow(i);
        // 逐列设置数据
        memberTable->setItem(i, 0, new QTableWidgetItem(member.id()));
        memberTable->setItem(i, 1, new QTableWidgetItem(member.name()));
        memberTable->setItem(i, 2, new QTableWidgetItem(member.phone()));
        memberTable->setItem(i, 3, new QTableWidgetItem(member.joinDate()));
    }
}

// 刷新课程表格
void MainWindow::refreshCourseTable()
{
    // 清空表格（保留列名）
    courseTable->setRowCount(0);
    // 获取所有课程数据
    QList<Course> courses = dataManager->getAllCourses();
    // 填充表格
    for (int i = 0; i < courses.size(); ++i) {
        Course course = courses[i];
        courseTable->insertRow(i);
        // 逐列设置数据
        courseTable->setItem(i, 0, new QTableWidgetItem(course.id()));
        courseTable->setItem(i, 1, new QTableWidgetItem(course.name()));
        courseTable->setItem(i, 2, new QTableWidgetItem(course.teacher()));
        courseTable->setItem(i, 3, new QTableWidgetItem(course.time()));
        courseTable->setItem(i, 4, new QTableWidgetItem(QString::number(course.maxNum())));
    }
}

// 点击「添加会员」按钮
void MainWindow::onAddMemberClicked()
{
    AddEditMemberDialog dialog(AddEditMemberDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户输入的会员数据
        Member member = dialog.getMemberData();
        // 添加到数据管理类
        if (dataManager->addMember(member)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员添加成功！"));
            refreshMemberTable(); // 刷新表格
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("会员ID已存在，请更换ID！"));
        }
    }
}

// 点击「编辑会员」按钮
void MainWindow::onEditMemberClicked()
{
    // 获取选中的行
    int selectedRow = memberTable->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选中要编辑的会员！"));
        return;
    }
    // 获取选中会员的ID
    QString memberId = memberTable->item(selectedRow, 0)->text();
    // 获取会员数据
    Member member = dataManager->getMemberById(memberId);
    // 打开编辑对话框
    AddEditMemberDialog dialog(AddEditMemberDialog::EditMode, member, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取修改后的会员数据
        Member newMember = dialog.getMemberData();
        // 编辑数据
        if (dataManager->editMember(memberId, newMember)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员编辑成功！"));
            refreshMemberTable(); // 刷新表格
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("编辑失败，请重试！"));
        }
    }
}

// 点击「删除会员」按钮
void MainWindow::onDeleteMemberClicked()
{
    // 获取选中的行
    int selectedRow = memberTable->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选中要删除的会员！"));
        return;
    }
    // 确认删除
    if (QMessageBox::question(this, QStringLiteral("确认"), QStringLiteral("确定要删除该会员吗？"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }
    // 获取选中会员的ID
    QString memberId = memberTable->item(selectedRow, 0)->text();
    // 删除数据
    if (dataManager->deleteMember(memberId)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员删除成功！"));
        refreshMemberTable(); // 刷新表格
    } else {
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("删除失败，请重试！"));
    }
}

// 点击「添加课程」按钮
void MainWindow::onAddCourseClicked()
{
    AddEditCourseDialog dialog(AddEditCourseDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户输入的课程数据
        Course course = dialog.getCourseData();
        // 添加到数据管理类
        if (dataManager->addCourse(course)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程添加成功！"));
            refreshCourseTable(); // 刷新表格
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("课程ID已存在，请更换ID！"));
        }
    }
}

// 点击「编辑课程」按钮
void MainWindow::onEditCourseClicked()
{
    // 获取选中的行
    int selectedRow = courseTable->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选中要编辑的课程！"));
        return;
    }
    // 获取选中课程的ID
    QString courseId = courseTable->item(selectedRow, 0)->text();
    // 获取课程数据
    Course course = dataManager->getCourseById(courseId);
    // 打开编辑对话框
    AddEditCourseDialog dialog(AddEditCourseDialog::EditMode, course, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取修改后的课程数据
        Course newCourse = dialog.getCourseData();
        // 编辑数据
        if (dataManager->editCourse(courseId, newCourse)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程编辑成功！"));
            refreshCourseTable(); // 刷新表格
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("编辑失败，请重试！"));
        }
    }
}

// 点击「删除课程」按钮
void MainWindow::onDeleteCourseClicked()
{
    // 获取选中的行
    int selectedRow = courseTable->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选中要删除的课程！"));
        return;
    }
    // 确认删除
    if (QMessageBox::question(this, QStringLiteral("确认"), QStringLiteral("确定要删除该课程吗？"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }
    // 获取选中课程的ID
    QString courseId = courseTable->item(selectedRow, 0)->text();
    // 删除数据
    if (dataManager->deleteCourse(courseId)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程删除成功！"));
        refreshCourseTable(); // 刷新表格
    } else {
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("删除失败，请重试！"));
    }
}
