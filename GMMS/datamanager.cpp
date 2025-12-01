#include "DataManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

DataManager* DataManager::m_instance = nullptr;

DataManager::DataManager(QObject *parent) : QObject(parent) {
    loadData();
}

DataManager::~DataManager() {
    saveData();
}

DataManager* DataManager::getInstance() {
    if (!m_instance) {
        m_instance = new DataManager();
    }
    return m_instance;
}

bool DataManager::loadData() {
    QFile file(DATA_FILE_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open data file for reading:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in data file";
        return false;
    }

    QJsonObject rootObj = doc.object();

    // 解析原有数据
    QJsonArray membersArray = rootObj[QStringLiteral("members")].toArray();
    QJsonArray coursesArray = rootObj[QStringLiteral("courses")].toArray();

    // 解析新增数据
    QJsonArray checkInsArray = rootObj[QStringLiteral("checkIns")].toArray();
    QJsonArray reservationsArray = rootObj[QStringLiteral("reservations")].toArray();

    // 加载会员
    m_members.clear();
    for (const QJsonValue& val : membersArray) {
        if (val.isObject()) {
            m_members.append(Member::fromJson(val.toObject()));
        }
    }

    // 加载课程
    m_courses.clear();
    for (const QJsonValue& val : coursesArray) {
        if (val.isObject()) {
            m_courses.append(Course::fromJson(val.toObject()));
        }
    }

    // 加载签到记录（新增）
    m_checkIns.clear();
    for (const QJsonValue& val : checkInsArray) {
        if (val.isObject()) {
            m_checkIns.append(CheckIn::fromJson(val.toObject()));
        }
    }

    // 加载预约记录（新增）
    m_reservations.clear();
    for (const QJsonValue& val : reservationsArray) {
        if (val.isObject()) {
            m_reservations.append(Reservation::fromJson(val.toObject()));
        }
    }

    qInfo() << "Loaded" << m_members.size() << "members,"
            << m_courses.size() << "courses,"
            << m_checkIns.size() << "check-ins,"
            << m_reservations.size() << "reservations";
    return true;
}

bool DataManager::saveData() {
    QJsonObject rootObj;
    QJsonArray membersArray;
    QJsonArray coursesArray;
    QJsonArray checkInsArray;    // 新增
    QJsonArray reservationsArray;// 新增

    // 序列化会员
    for (const Member& member : m_members) {
        membersArray.append(member.toJson());
    }

    // 序列化课程
    for (const Course& course : m_courses) {
        coursesArray.append(course.toJson());
    }

    // 序列化签到记录（新增）
    for (const CheckIn& checkIn : m_checkIns) {
        checkInsArray.append(checkIn.toJson());
    }

    // 序列化预约记录（新增）
    for (const Reservation& reservation : m_reservations) {
        reservationsArray.append(reservation.toJson());
    }

    rootObj[QStringLiteral("members")] = membersArray;
    rootObj[QStringLiteral("courses")] = coursesArray;
    rootObj[QStringLiteral("checkIns")] = checkInsArray;       // 新增
    rootObj[QStringLiteral("reservations")] = reservationsArray; // 新增

    QJsonDocument doc(rootObj);
    QFile file(DATA_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open data file for writing:" << file.errorString();
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

// 会员管理（新增查询功能）
QList<Member> DataManager::getAllMembers() const { return m_members; }

bool DataManager::addMember(const Member& member) {
    for (const Member& m : m_members) {
        if (m.id() == member.id()) return false;
    }
    m_members.append(member);
    return saveData();
}

bool DataManager::editMember(const QString& oldId, const Member& newMember) {
    for (int i = 0; i < m_members.size(); ++i) {
        if (m_members[i].id() == oldId) {
            if (newMember.id() != oldId) {
                for (const Member& m : m_members) {
                    if (m.id() == newMember.id()) return false;
                }
            }
            m_members[i] = newMember;
            return saveData();
        }
    }
    return false;
}

bool DataManager::deleteMember(const QString& id) {
    for (int i = 0; i < m_members.size(); ++i) {
        if (m_members[i].id() == id) {
            m_members.removeAt(i);
            return saveData();
        }
    }
    return false;
}

Member DataManager::getMemberById(const QString& id) const {
    for (const Member& m : m_members) {
        if (m.id() == id) return m;
    }
    return Member();
}

// 新增会员查询功能（按ID或姓名模糊查询）
QList<Member> DataManager::searchMembers(const QString& keyword) const {
    QList<Member> result;
    QString key = keyword.trimmed().toLower();
    for (const Member& m : m_members) {
        if (m.id().toLower().contains(key) || m.name().toLower().contains(key)) {
            result.append(m);
        }
    }
    return result;
}

// 课程管理（保持不变）
QList<Course> DataManager::getAllCourses() const { return m_courses; }
bool DataManager::addCourse(const Course& course) {
    for (const Course& c : m_courses) {
        if (c.id() == course.id()) return false;
    }
    m_courses.append(course);
    return saveData();
}
bool DataManager::editCourse(const QString& oldId, const Course& newCourse) {
    for (int i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].id() == oldId) {
            if (newCourse.id() != oldId) {
                for (const Course& c : m_courses) {
                    if (c.id() == newCourse.id()) return false;
                }
            }
            m_courses[i] = newCourse;
            return saveData();
        }
    }
    return false;
}
bool DataManager::deleteCourse(const QString& id) {
    for (int i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].id() == id) {
            m_courses.removeAt(i);
            return saveData();
        }
    }
    return false;
}
Course DataManager::getCourseById(const QString& id) const {
    for (const Course& c : m_courses) {
        if (c.id() == id) return c;
    }
    return Course();
}

// 签到管理（新增实现）
QList<CheckIn> DataManager::getAllCheckIns() const { return m_checkIns; }

bool DataManager::addCheckIn(const CheckIn& checkIn) {
    m_checkIns.append(checkIn);
    return saveData();
}

QList<CheckIn> DataManager::getCheckInsByMemberId(const QString& memberId) const {
    QList<CheckIn> result;
    for (const CheckIn& ci : m_checkIns) {
        if (ci.memberId() == memberId) {
            result.append(ci);
        }
    }
    return result;
}

// 预约管理（新增实现）
QList<Reservation> DataManager::getAllReservations() const { return m_reservations; }

bool DataManager::addReservation(const Reservation& reservation) {
    // 检查是否已预约
    if (isReserved(reservation.memberId(), reservation.courseId())) {
        return false;
    }
    m_reservations.append(reservation);
    return saveData();
}

bool DataManager::cancelReservation(const QString& memberId, const QString& courseId) {
    for (int i = 0; i < m_reservations.size(); ++i) {
        if (m_reservations[i].memberId() == memberId &&
            m_reservations[i].courseId() == courseId) {
            m_reservations.removeAt(i);
            return saveData();
        }
    }
    return false;
}

bool DataManager::isReserved(const QString& memberId, const QString& courseId) const {
    for (const Reservation& r : m_reservations) {
        if (r.memberId() == memberId && r.courseId() == courseId) {
            return true;
        }
    }
    return false;
}
