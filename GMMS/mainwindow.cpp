#include "MainWindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDate>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dataManager(DataManager::getInstance()) {
    setWindowTitle(QStringLiteral("健身房会员管理系统"));
    setMinimumSize(1200, 700);
    initUI();
}

MainWindow::~MainWindow() {}

void MainWindow::initUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    tabWidget = new QTabWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(tabWidget);

    // 初始化各标签页
    initMemberTab();
    initCourseTab();
    initCheckInTab();

    tabWidget->addTab(memberTab, QStringLiteral("会员管理"));
    tabWidget->addTab(courseTab, QStringLiteral("课程管理"));
    tabWidget->addTab(checkInTab, QStringLiteral("签到与预约"));
}

void MainWindow::initMemberTab() {
    memberTab = new QWidget(tabWidget);
    QVBoxLayout *layout = new QVBoxLayout(memberTab);

    // 查询区域
    QHBoxLayout *searchLayout = new QHBoxLayout;
    memberSearchEdit = new QLineEdit;
    memberSearchEdit->setPlaceholderText(QStringLiteral("输入会员ID或姓名查询..."));
    searchMemberBtn = new QPushButton(QStringLiteral("查询"));
    searchLayout->addWidget(memberSearchEdit);
    searchLayout->addWidget(searchMemberBtn);

    // 操作按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    addMemberBtn = new QPushButton(QStringLiteral("添加会员"));
    editMemberBtn = new QPushButton(QStringLiteral("编辑会员"));
    deleteMemberBtn = new QPushButton(QStringLiteral("删除会员"));
    btnLayout->addWidget(addMemberBtn);
    btnLayout->addWidget(editMemberBtn);
    btnLayout->addWidget(deleteMemberBtn);
    btnLayout->addStretch();

    // 会员表格
    memberTable = new QTableWidget(memberTab);
    memberTable->setColumnCount(5);
    memberTable->setHorizontalHeaderLabels({
        QStringLiteral("会员ID"),
        QStringLiteral("姓名"),
        QStringLiteral("联系电话"),
        QStringLiteral("加入日期"),
        QStringLiteral("有效期至")
    });
    memberTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 布局组合
    layout->addLayout(searchLayout);
    layout->addLayout(btnLayout);
    layout->addWidget(memberTable);

    // 信号连接
    connect(searchMemberBtn, &QPushButton::clicked, this, &MainWindow::onSearchMemberClicked);
    connect(addMemberBtn, &QPushButton::clicked, this, &MainWindow::onAddMemberClicked);
    connect(editMemberBtn, &QPushButton::clicked, this, &MainWindow::onEditMemberClicked);
    connect(deleteMemberBtn, &QPushButton::clicked, this, &MainWindow::onDeleteMemberClicked);

    refreshMemberTable();
}

void MainWindow::initCourseTab() {
    courseTab = new QWidget(tabWidget);
    QVBoxLayout *layout = new QVBoxLayout(courseTab);

    // 操作按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    addCourseBtn = new QPushButton(QStringLiteral("添加课程"));
    editCourseBtn = new QPushButton(QStringLiteral("编辑课程"));
    deleteCourseBtn = new QPushButton(QStringLiteral("删除课程"));
    btnLayout->addWidget(addCourseBtn);
    btnLayout->addWidget(editCourseBtn);
    btnLayout->addWidget(deleteCourseBtn);
    btnLayout->addStretch();

    // 课程表格
    courseTable = new QTableWidget(courseTab);
    courseTable->setColumnCount(3);
    courseTable->setHorizontalHeaderLabels({
        QStringLiteral("课程ID"),
        QStringLiteral("课程名称"),
        QStringLiteral("课程时间")
    });
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addLayout(btnLayout);
    layout->addWidget(courseTable);

    refreshCourseTable();
}

void MainWindow::initCheckInTab() {
    checkInTab = new QWidget(tabWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(checkInTab);

    // 签到区域
    QGroupBox *checkInGroup = new QGroupBox(QStringLiteral("会员签到"), checkInTab);
    QHBoxLayout *checkInLayout = new QHBoxLayout(checkInGroup);
    checkInLayout->addWidget(new QLabel(QStringLiteral("会员ID:")));
    checkInMemberIdEdit = new QLineEdit;
    checkInLayout->addWidget(checkInMemberIdEdit);
    checkInLayout->addWidget(new QLabel(QStringLiteral("课程ID:")));
    checkInCourseIdEdit = new QLineEdit;
    checkInLayout->addWidget(checkInCourseIdEdit);
    checkInBtn = new QPushButton(QStringLiteral("确认签到"));
    checkInLayout->addWidget(checkInBtn);

    // 预约区域
    QGroupBox *reserveGroup = new QGroupBox(QStringLiteral("课程预约"), checkInTab);
    QHBoxLayout *reserveLayout = new QHBoxLayout(reserveGroup);
    reserveLayout->addWidget(new QLabel(QStringLiteral("会员ID:")));
    reserveMemberIdEdit = new QLineEdit;
    reserveLayout->addWidget(reserveMemberIdEdit);
    reserveLayout->addWidget(new QLabel(QStringLiteral("课程ID:")));
    reserveCourseIdEdit = new QLineEdit;
    reserveLayout->addWidget(reserveCourseIdEdit);
    reserveBtn = new QPushButton(QStringLiteral("预约课程"));
    cancelReserveBtn = new QPushButton(QStringLiteral("取消预约"));
    reserveLayout->addWidget(reserveBtn);
    reserveLayout->addWidget(cancelReserveBtn);

    // 记录表格
    QTabWidget *recordTab = new QTabWidget(checkInTab);
    checkInTable = new QTableWidget;
    checkInTable->setColumnCount(4);
    checkInTable->setHorizontalHeaderLabels({
        QStringLiteral("会员ID"),
        QStringLiteral("会员姓名"),
        QStringLiteral("课程ID"),
        QStringLiteral("签到时间")
    });
    checkInTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    reservationTable = new QTableWidget;
    reservationTable->setColumnCount(4);
    reservationTable->setHorizontalHeaderLabels({
        QStringLiteral("会员ID"),
        QStringLiteral("会员姓名"),
        QStringLiteral("课程ID"),
        QStringLiteral("预约时间")
    });
    reservationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    recordTab->addTab(checkInTable, QStringLiteral("签到记录"));
    recordTab->addTab(reservationTable, QStringLiteral("预约记录"));

    // 布局组合
    mainLayout->addWidget(checkInGroup);
    mainLayout->addWidget(reserveGroup);
    mainLayout->addWidget(recordTab);

    // 信号连接
    connect(checkInBtn, &QPushButton::clicked, this, &MainWindow::onCheckInClicked);
    connect(reserveBtn, &QPushButton::clicked, this, &MainWindow::onReserveCourseClicked);
    connect(cancelReserveBtn, &QPushButton::clicked, this, &MainWindow::onCancelReservationClicked);

    refreshCheckInTable();
    refreshReservationTable();
}

// 会员管理相关
void MainWindow::refreshMemberTable() {
    memberTable->setRowCount(0);
    QList<Member> members = dataManager->getAllMembers();
    for (int i = 0; i < members.size(); ++i) {
        const Member& m = members[i];
        memberTable->insertRow(i);
        memberTable->setItem(i, 0, new QTableWidgetItem(m.id()));
        memberTable->setItem(i, 1, new QTableWidgetItem(m.name()));
        memberTable->setItem(i, 2, new QTableWidgetItem(m.phone()));
        memberTable->setItem(i, 3, new QTableWidgetItem(m.joinDate()));
        memberTable->setItem(i, 4, new QTableWidgetItem(m.expiryDate()));
    }
}

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

void MainWindow::onEditMemberClicked() {
    int row = memberTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选择要编辑的会员！"));
        return;
    }
    QString memberId = memberTable->item(row, 0)->text();
    Member member = dataManager->getMemberById(memberId);

    AddEditMemberDialog dialog(AddEditMemberDialog::EditMode, member, this);
    if (dialog.exec() == QDialog::Accepted) {
        Member newMember = dialog.getMember();
        if (dataManager->editMember(memberId, newMember)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员编辑成功！"));
            refreshMemberTable();
        }
    }
}

void MainWindow::onDeleteMemberClicked() {
    int row = memberTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请先选择要删除的会员！"));
        return;
    }
    QString memberId = memberTable->item(row, 0)->text();
    if (QMessageBox::question(this, QStringLiteral("确认"), QStringLiteral("确定要删除该会员吗？")) == QMessageBox::Yes) {
        if (dataManager->deleteMember(memberId)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("会员删除成功！"));
            refreshMemberTable();
        }
    }
}

void MainWindow::onSearchMemberClicked() {
    QString keyword = memberSearchEdit->text().trimmed();
    QList<Member> results = dataManager->searchMembers(keyword);

    memberTable->setRowCount(0);
    for (int i = 0; i < results.size(); ++i) {
        const Member& m = results[i];
        memberTable->insertRow(i);
        memberTable->setItem(i, 0, new QTableWidgetItem(m.id()));
        memberTable->setItem(i, 1, new QTableWidgetItem(m.name()));
        memberTable->setItem(i, 2, new QTableWidgetItem(m.phone()));
        memberTable->setItem(i, 3, new QTableWidgetItem(m.joinDate()));
        memberTable->setItem(i, 4, new QTableWidgetItem(m.expiryDate()));
    }
}

// 课程管理相关
void MainWindow::refreshCourseTable() {
    courseTable->setRowCount(0);
    QList<Course> courses = dataManager->getAllCourses();
    for (int i = 0; i < courses.size(); ++i) {
        const Course& c = courses[i];
        courseTable->insertRow(i);
        courseTable->setItem(i, 0, new QTableWidgetItem(c.id()));
        courseTable->setItem(i, 1, new QTableWidgetItem(c.name()));
        courseTable->setItem(i, 2, new QTableWidgetItem(c.time()));
    }
}

// 签到与预约相关
void MainWindow::refreshCheckInTable() {
    checkInTable->setRowCount(0);
    QList<CheckIn> checkIns = dataManager->getAllCheckIns();
    for (int i = 0; i < checkIns.size(); ++i) {
        const CheckIn& ci = checkIns[i];
        checkInTable->insertRow(i);
        checkInTable->setItem(i, 0, new QTableWidgetItem(ci.memberId()));
        checkInTable->setItem(i, 1, new QTableWidgetItem(dataManager->getMemberById(ci.memberId()).name()));
        checkInTable->setItem(i, 2, new QTableWidgetItem(ci.courseId()));
        checkInTable->setItem(i, 3, new QTableWidgetItem(ci.checkInTime().toString("yyyy-MM-dd HH:mm")));
    }
}

void MainWindow::refreshReservationTable() {
    reservationTable->setRowCount(0);
    QList<Reservation> reservations = dataManager->getAllReservations();
    for (int i = 0; i < reservations.size(); ++i) {
        const Reservation& r = reservations[i];
        reservationTable->insertRow(i);
        reservationTable->setItem(i, 0, new QTableWidgetItem(r.memberId()));
        reservationTable->setItem(i, 1, new QTableWidgetItem(dataManager->getMemberById(r.memberId()).name()));
        reservationTable->setItem(i, 2, new QTableWidgetItem(r.courseId()));
        reservationTable->setItem(i, 3, new QTableWidgetItem(r.reserveTime().toString("yyyy-MM-dd HH:mm")));
    }
}

void MainWindow::onCheckInClicked() {
    QString memberId = checkInMemberIdEdit->text().trimmed();
    QString courseId = checkInCourseIdEdit->text().trimmed();

    if (memberId.isEmpty() || courseId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员ID和课程ID不能为空！"));
        return;
    }

    // 检查会员是否存在且有效
    Member member = dataManager->getMemberById(memberId);
    if (member.id().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员不存在！"));
        return;
    }

    // 检查会员是否过期
    QDate expiryDate = QDate::fromString(member.expiryDate(), "yyyy-MM-dd");
    if (expiryDate < QDate::currentDate()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员已过期！"));
        return;
    }

    // 检查课程是否存在
    Course course = dataManager->getCourseById(courseId);
    if (course.id().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("课程不存在！"));
        return;
    }

    // 执行签到
    CheckIn checkIn(memberId, courseId, QDateTime::currentDateTime());
    if (dataManager->addCheckIn(checkIn)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("签到成功！"));
        refreshCheckInTable();
        checkInMemberIdEdit->clear();
        checkInCourseIdEdit->clear();
    }
}

void MainWindow::onReserveCourseClicked() {
    QString memberId = reserveMemberIdEdit->text().trimmed();
    QString courseId = reserveCourseIdEdit->text().trimmed();

    if (memberId.isEmpty() || courseId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员ID和课程ID不能为空！"));
        return;
    }

    // 检查会员和课程是否存在
    if (dataManager->getMemberById(memberId).id().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员不存在！"));
        return;
    }
    if (dataManager->getCourseById(courseId).id().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("课程不存在！"));
        return;
    }

    // 检查是否已预约
    if (dataManager->isReserved(memberId, courseId)) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("已预约该课程！"));
        return;
    }

    // 执行预约
    Reservation reservation(memberId, courseId, QDateTime::currentDateTime());
    if (dataManager->addReservation(reservation)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("预约成功！"));
        refreshReservationTable();
        reserveMemberIdEdit->clear();
        reserveCourseIdEdit->clear();
    }
}

void MainWindow::onCancelReservationClicked() {
    QString memberId = reserveMemberIdEdit->text().trimmed();
    QString courseId = reserveCourseIdEdit->text().trimmed();

    if (memberId.isEmpty() || courseId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("会员ID和课程ID不能为空！"));
        return;
    }

    if (dataManager->cancelReservation(memberId, courseId)) {
        QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("取消预约成功！"));
        refreshReservationTable();
        reserveMemberIdEdit->clear();
        reserveCourseIdEdit->clear();
    } else {
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("未找到该预约记录！"));
    }
}
