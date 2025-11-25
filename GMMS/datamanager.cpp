#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    // 初始化时加载数据（如果文件不存在，会自动创建空文件）
    if (!loadData()) {
        // 加载失败时创建空数据文件
        saveData();
    }
}

DataManager::~DataManager()
{
    // 析构时保存数据（确保修改不丢失）
    saveData();
}

// 获取所有会员
QList<Member> DataManager::getAllMembers()
{
    return m_members;
}

// 添加会员
bool DataManager::addMember(const Member &member)
{
    // 检查ID是否已存在
    if (isMemberIdExists(member.id())) {
        return false;
    }
    // 添加到内存
    m_members.append(member);
    // 保存到文件
    return saveData();
}

// 编辑会员
bool DataManager::editMember(const QString &id, const Member &newMember)
{
    // 查找会员索引
    for (int i = 0; i < m_members.size(); ++i) {
        if (m_members[i].id() == id) {
            // 替换为新数据（允许修改ID，但需确保新ID不重复）
            if (newMember.id() != id && isMemberIdExists(newMember.id())) {
                return false;
            }
            m_members[i] = newMember;
            return saveData();
        }
    }
    return false; // 未找到该会员
}

// 删除会员
bool DataManager::deleteMember(const QString &id)
{
    // 查找并删除会员
    for (int i = 0; i < m_members.size(); ++i) {
        if (m_members[i].id() == id) {
            m_members.removeAt(i);
            return saveData();
        }
    }
    return false; // 未找到该会员
}

// 根据ID获取会员
Member DataManager::getMemberById(const QString &id)
{
    for (const Member &member : m_members) {
        if (member.id() == id) {
            return member;
        }
    }
    return Member(); // 未找到返回空会员
}

// 检查会员ID是否存在
bool DataManager::isMemberIdExists(const QString &id)
{
    for (const Member &member : m_members) {
        if (member.id() == id) {
            return true;
        }
    }
    return false;
}

// 获取所有课程
QList<Course> DataManager::getAllCourses()
{
    return m_courses;
}

// 添加课程
bool DataManager::addCourse(const Course &course)
{
    // 检查ID是否已存在
    if (isCourseIdExists(course.id())) {
        return false;
    }
    // 添加到内存
    m_courses.append(course);
    // 保存到文件
    return saveData();
}

// 编辑课程
bool DataManager::editCourse(const QString &id, const Course &newCourse)
{
    // 查找课程索引
    for (int i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].id() == id) {
            // 替换为新数据（允许修改ID，但需确保新ID不重复）
            if (newCourse.id() != id && isCourseIdExists(newCourse.id())) {
                return false;
            }
            m_courses[i] = newCourse;
            return saveData();
        }
    }
    return false; // 未找到该课程
}

// 删除课程
bool DataManager::deleteCourse(const QString &id)
{
    // 查找并删除课程
    for (int i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].id() == id) {
            m_courses.removeAt(i);
            return saveData();
        }
    }
    return false; // 未找到该课程
}

// 根据ID获取课程
Course DataManager::getCourseById(const QString &id)
{
    for (const Course &course : m_courses) {
        if (course.id() == id) {
            return course;
        }
    }
    return Course(); // 未找到返回空课程
}

// 检查课程ID是否存在
bool DataManager::isCourseIdExists(const QString &id)
{
    for (const Course &course : m_courses) {
        if (course.id() == id) {
            return true;
        }
    }
    return false;
}

// 从JSON文件加载数据
bool DataManager::loadData()
{
    // 检查文件是否存在
    QFile file(DATA_FILE_PATH);
    if (!file.exists()) {
        return false;
    }
    // 打开文件（只读模式）
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();
    // 解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }
    QJsonObject rootObj = doc.object();

    // 解析会员数据
    QJsonArray membersArray = rootObj["members"].toArray();
    m_members.clear();
    for (const QJsonValue &val : membersArray) {
        if (val.isObject()) {
            m_members.append(Member::fromJson(val.toObject()));
        }
    }

    // 解析课程数据
    QJsonArray coursesArray = rootObj["courses"].toArray();
    m_courses.clear();
    for (const QJsonValue &val : coursesArray) {
        if (val.isObject()) {
            m_courses.append(Course::fromJson(val.toObject()));
        }
    }

    return true;
}

// 将数据保存到JSON文件
bool DataManager::saveData()
{
    // 创建JSON根对象
    QJsonObject rootObj;

    // 会员数据转换为JSON数组
    QJsonArray membersArray;
    for (const Member &member : m_members) {
        membersArray.append(member.toJson());
    }
    rootObj["members"] = membersArray;

    // 课程数据转换为JSON数组
    QJsonArray coursesArray;
    for (const Course &course : m_courses) {
        coursesArray.append(course.toJson());
    }
    rootObj["courses"] = coursesArray;

    // 生成JSON文档
    QJsonDocument doc(rootObj);
    QByteArray data = doc.toJson(QJsonDocument::Indented); // 格式化输出（易读）

    // 打开文件（写入模式，不存在则创建）
    QFile file(DATA_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    // 写入文件
    file.write(data);
    file.close();

    return true;
}
