#include "DataManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

// 初始化单例
DataManager* DataManager::m_instance = nullptr;

DataManager::DataManager(QObject *parent) : QObject(parent) {
    // 初始化时加载数据
    loadData();
}

DataManager::~DataManager() {
    // 析构时保存数据
    saveData();
}

DataManager* DataManager::getInstance() {
    if (!m_instance) {
        m_instance = new DataManager();
    }
    return m_instance;
}

// 加载数据
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
    QJsonArray membersArray = rootObj[QStringLiteral("members")].toArray();
    QJsonArray coursesArray = rootObj[QStringLiteral("courses")].toArray();

    // 解析会员数据
    m_members.clear();
    for (const QJsonValue& val : membersArray) {
        if (val.isObject()) {
            m_members.append(Member::fromJson(val.toObject()));
        }
    }

    // 解析课程数据
    m_courses.clear();
    for (const QJsonValue& val : coursesArray) {
        if (val.isObject()) {
            m_courses.append(Course::fromJson(val.toObject()));
        }
    }

    qInfo() << "Loaded" << m_members.size() << "members and" << m_courses.size() << "courses";
    return true;
}

// 保存数据
bool DataManager::saveData() {
    QJsonObject rootObj;
    QJsonArray membersArray;
    QJsonArray coursesArray;

    // 序列化会员数据
    for (const Member& member : m_members) {
        membersArray.append(member.toJson());
    }

    // 序列化课程数据
    for (const Course& course : m_courses) {
        coursesArray.append(course.toJson());
    }

    rootObj[QStringLiteral("members")] = membersArray;
    rootObj[QStringLiteral("courses")] = coursesArray;

    QJsonDocument doc(rootObj);
    QFile file(DATA_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open data file for writing:" << file.errorString();
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qInfo() << "Saved" << m_members.size() << "members and" << m_courses.size() << "courses";
    return true;
}

// 会员管理实现
QList<Member> DataManager::getAllMembers() const {
    return m_members;
}

bool DataManager::addMember(const Member& member) {
    // 检查ID是否重复
    for (const Member& m : m_members) {
        if (m.id() == member.id()) {
            return false;
        }
    }
    m_members.append(member);
    return saveData();
}

bool DataManager::editMember(const QString& oldId, const Member& newMember) {
    for (int i = 0; i < m_members.size(); ++i) {
        if (m_members[i].id() == oldId) {
            // 检查新ID是否与其他会员冲突
            if (newMember.id() != oldId) {
                for (const Member& m : m_members) {
                    if (m.id() == newMember.id()) {
                        return false;
                    }
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
        if (m.id() == id) {
            return m;
        }
    }
    return Member(); // 返回空对象
}

// 课程管理实现
QList<Course> DataManager::getAllCourses() const {
    return m_courses;
}

bool DataManager::addCourse(const Course& course) {
    // 检查ID是否重复
    for (const Course& c : m_courses) {
        if (c.id() == course.id()) {
            return false;
        }
    }
    m_courses.append(course);
    return saveData();
}

bool DataManager::editCourse(const QString& oldId, const Course& newCourse) {
    for (int i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].id() == oldId) {
            // 检查新ID是否与其他课程冲突
            if (newCourse.id() != oldId) {
                for (const Course& c : m_courses) {
                    if (c.id() == newCourse.id()) {
                        return false;
                    }
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
        if (c.id() == id) {
            return c;
        }
    }
    return Course(); // 返回空对象
}
