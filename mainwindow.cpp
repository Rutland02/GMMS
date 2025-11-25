#include "MainWindow.h"
#include "AddEditMemberDialog.h"
#include "AddEditCourseDialog.h"
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dataManager(DataManager::getInstance()) {
    setWindowTitle(QStringLiteral("健身房会员管理系统（GMMS）"));
    setMinimumSize(800, 600);
    initUI();
}

MainWindow::~MainWindow() {}

void MainWindow::initUI() {
    // 中心窗口和标签页
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    tabWidget = new QTabWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(tabWidget);

    // 初始化会员标签页和课程标签页
    initMemberTab();
    initCourseTab();

    // 添加标签页
    tabWidget->addTab(memberTab, QStringLiteral("会员管理"));
    tabWidget->addTab(courseTab, QStringLiteral("课程管理"));
}

void MainWindow::initMemberTab() {
    memberTab = new QWidget(tabWidget);
    QVBoxLayout *layout = new QVBoxLayout(memberTab);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    addMemberBtn = new QPushButton(QStringLiteral("添加会员"), memberTab);
    editMemberBtn = new QPushButton(QStringLiteral("编辑会员"), memberTab);
    deleteMemberBtn = new QPushButton(QStringLiteral("删除会员"), memberTab);
    btnLayout->addWidget(addMemberBtn);
    btnLayout->addWidget(editMemberBtn);
    btnLayout->addWidget(deleteMemberBtn);
    btnLayout->addStretch();

    // 会员表格
    memberTable = new QTableWidget(memberTab);
    memberTable->setColumnCount(4);
    memberTable->setHorizontalHeaderLabels({
        QStringLiteral("会员ID"),
        QStringLiteral("姓名"),
        QStringLiteral("联系电话"),
        QStringLiteral("加入日期")
    });
    memberTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    memberTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    memberTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自适应

    // 添加到布局
    layout->addLayout(btnLayout);
    layout->addWidget(memberTable);

    // 连接信号槽
    connect(addMemberBtn, &QPushButton::clicked, this, &MainWindow::onAddMemberClicked);
    connect(editMemberBtn, &QPushButton::clicked, this, &MainWindow::onEditMemberClicked);
    connect(deleteMemberBtn, &QPushButton::clicked, this, &MainWindow::onDeleteMemberClicked);

    // 初始刷新表格
    refreshMemberTable();
}

void MainWindow::initCourseTab() {
    courseTab = new QWidget(tabWidget);
    QVBoxLayout *layout = new QVBoxLayout(courseTab);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    addCourseBtn = new QPushButton(QStringLiteral("添加课程"), courseTab);
    editCourseBtn = new QPushButton(QStringLiteral("编辑课程"), courseTab);
    deleteCourseBtn = new QPushButton(QStringLiteral("删除课程"), courseTab);
    btnLayout->addWidget(addCourseBtn);
    btnLayout->addWidget(editCourseBtn);
    btnLayout->addWidget(deleteCourseBtn);
    btnLayout->addStretch();

    // 课程表格
    courseTable = new QTableWidget(courseTab);
    courseTable->setColumnCount(5);
    courseTable->setHorizontalHeaderLabels({
        QStringLiteral("课程ID"),
        QStringLiteral("课程名称"),
        QStringLiteral("授课老师"),
        QStringLiteral("上课时间"),
        QStringLiteral("最大人数")
    });
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 列宽自适应

    // 添加到布局
    layout->addLayout(btnLayout);
    layout->addWidget(courseTable);

    // 连接信号槽
    connect(addCourseBtn, &QPushButton::clicked, this, &MainWindow::onAddCourseClicked);
    connect(editCourseBtn, &QPushButton::clicked, this, &MainWindow::onEditCourseClicked);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &MainWindow::onDeleteCourseClicked);

    // 初始刷新表格
    refreshCourseTable();
}

// 会员表格刷新
void MainWindow::refreshMemberTable() {
    // 清空表格
    memberTable->setRowCount(0);

    // 获取所有会员
    QList<Member> members = dataManager->getAllMembers();
    for (int i = 0; i < members.size(); ++i) {
        const Member& member = members[i];
        memberTable->insertRow(i);
        memberTable->setItem(i, 0, new QTableWidgetItem(member.id()));
        memberTable->setItem(i, 1, new QTableWidgetItem(member.name()));
        memberTable->setItem(i, 2, new QTableWidgetItem(member.phone()));
        memberTable->setItem(i, 3, new QTableWidgetItem(member.joinDate()));
    }
}

// 课程表格刷新
void MainWindow::refreshCourseTable() {
    // 清空表格
    courseTable->setRowCount(0);

    // 获取所有课程
    QList<Course> courses = dataManager->getAllCourses();
    for (int i = 0; i < courses.size(); ++i) {
        const Course& course = courses[i];
        courseTable->insertRow(i);
        courseTable->setItem(i, 0, new QTableWidgetItem(course.id()));
        courseTable->setItem(i, 1, new QTableWidgetItem(course.name()));
        courseTable->setItem(i, 2, new QTableWidgetItem(course.teacher()));
        courseTable->setItem(i, 3, new QTableWidgetItem(course.time()));
        courseTable->setItem(i, 4, new QTableWidgetItem(QString::number(course.maxNum())));
    }
}

// 添加会员
void MainWindow::onAddMemberClicked() {
    AddEditMemberDialog dialog(AddEditMemberDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        Member member = dialog.getMember();
        if (dataManager->addMember(member)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员添加成功！"));
            refreshMemberTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("会员ID已存在！"));
        }
    }
}

// 编辑会员
void MainWindow::onEditMemberClicked() {
    // 获取选中行
    int row = memberTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择要编辑的会员！"));
        return;
    }

    // 获取选中会员ID
    QString memberId = memberTable->item(row, 0)->text();
    Member member = dataManager->getMemberById(memberId);

    // 打开编辑对话框
    AddEditMemberDialog dialog(AddEditMemberDialog::EditMode, member, this);
    if (dialog.exec() == QDialog::Accepted) {
        Member newMember = dialog.getMember();
        if (dataManager->editMember(memberId, newMember)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员编辑成功！"));
            refreshMemberTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("编辑失败，请重试！"));
        }
    }
}

// 删除会员
void MainWindow::onDeleteMemberClicked() {
    // 获取选中行
    int row = memberTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择要删除的会员！"));
        return;
    }

    // 确认删除
    QString memberId = memberTable->item(row, 0)->text();
    QString memberName = memberTable->item(row, 1)->text();
    int ret = QMessageBox::question(
        this,
        QStringLiteral("确认删除"),
        QStringLiteral("确定要删除会员【%1（%2）】吗？").arg(memberName).arg(memberId),
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret == QMessageBox::Yes) {
        if (dataManager->deleteMember(memberId)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员删除成功！"));
            refreshMemberTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("删除失败，请重试！"));
        }
    }
}

// 添加课程
void MainWindow::onAddCourseClicked() {
    AddEditCourseDialog dialog(AddEditCourseDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        Course course = dialog.getCourse();
        if (dataManager->addCourse(course)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程添加成功！"));
            refreshCourseTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("课程ID已存在！"));
        }
    }
}

// 编辑课程
void MainWindow::onEditCourseClicked() {
    // 获取选中行
    int row = courseTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择要编辑的课程！"));
        return;
    }

    // 获取选中课程ID
    QString courseId = courseTable->item(row, 0)->text();
    Course course = dataManager->getCourseById(courseId);

    // 打开编辑对话框
    AddEditCourseDialog dialog(AddEditCourseDialog::EditMode, course, this);
    if (dialog.exec() == QDialog::Accepted) {
        Course newCourse = dialog.getCourse();
        if (dataManager->editCourse(courseId, newCourse)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程编辑成功！"));
            refreshCourseTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("编辑失败，请重试！"));
        }
    }
}

// 删除课程
void MainWindow::onDeleteCourseClicked() {
    // 获取选中行
    int row = courseTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请选择要删除的课程！"));
        return;
    }

    // 确认删除
    QString courseId = courseTable->item(row, 0)->text();
    QString courseName = courseTable->item(row, 1)->text();
    int ret = QMessageBox::question(
        this,
        QStringLiteral("确认删除"),
        QStringLiteral("确定要删除课程【%1（%2）】吗？").arg(courseName).arg(courseId),
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret == QMessageBox::Yes) {
        if (dataManager->deleteCourse(courseId)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程删除成功！"));
            refreshCourseTable();
        } else {
            QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("删除失败，请重试！"));
        }
    }
}
