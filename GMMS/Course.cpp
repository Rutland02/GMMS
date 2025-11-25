#include "Course.h"

// 带参构造函数实现
Course::Course(const QString& id, const QString& name, const QString& teacher, const QString& time, int maxNum)
    : m_id(id), m_name(name), m_teacher(teacher), m_time(time), m_maxNum(maxNum) {}

// JSON序列化
QJsonObject Course::toJson() const {
    QJsonObject obj;
    obj[QStringLiteral("id")] = m_id;
    obj[QStringLiteral("name")] = m_name;
    obj[QStringLiteral("teacher")] = m_teacher;
    obj[QStringLiteral("time")] = m_time;
    obj[QStringLiteral("maxNum")] = m_maxNum;
    return obj;
}

// JSON反序列化
Course Course::fromJson(const QJsonObject& obj) {
    Course course;
    course.setId(obj[QStringLiteral("id")].toString());
    course.setName(obj[QStringLiteral("name")].toString());
    course.setTeacher(obj[QStringLiteral("teacher")].toString());
    course.setTime(obj[QStringLiteral("time")].toString());
    course.setMaxNum(obj[QStringLiteral("maxNum")].toInt());
    return course;
}

// Getter实现
QString Course::id() const { return m_id; }
QString Course::name() const { return m_name; }
QString Course::teacher() const { return m_teacher; }
QString Course::time() const { return m_time; }
int Course::maxNum() const { return m_maxNum; }

// Setter实现
void Course::setId(const QString &id) { m_id = id; }
void Course::setName(const QString &name) { m_name = name; }
void Course::setTeacher(const QString &teacher) { m_teacher = teacher; }
void Course::setTime(const QString &time) { m_time = time; }
void Course::setMaxNum(int maxNum) { m_maxNum = maxNum; }
