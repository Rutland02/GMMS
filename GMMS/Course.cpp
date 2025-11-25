#include "Course.h"

Course::Course()
    : m_id(""), m_name(""), m_teacher(""), m_time(""), m_maxNum(0)
{
}

Course::Course(const QString &id, const QString &name, const QString &teacher, const QString &time, int maxNum)
    : m_id(id), m_name(name), m_teacher(teacher), m_time(time), m_maxNum(maxNum)
{
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

// 转换为JSON对象
QJsonObject Course::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["teacher"] = m_teacher;
    json["time"] = m_time;
    json["maxNum"] = m_maxNum;
    return json;
}

// 从JSON对象解析
Course Course::fromJson(const QJsonObject &json)
{
    Course course;
    course.setId(json["id"].toString());
    course.setName(json["name"].toString());
    course.setTeacher(json["teacher"].toString());
    course.setTime(json["time"].toString());
    course.setMaxNum(json["maxNum"].toInt());
    return course;
}
