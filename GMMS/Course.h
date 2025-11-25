#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QJsonObject>

class Course {
public:
    Course() = default;
    Course(const QString& id, const QString& name, const QString& teacher, const QString& time)
        : m_id(id), m_name(name), m_teacher(teacher), m_time(time) {}

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = m_id;
        obj["name"] = m_name;
        obj["teacher"] = m_teacher;
        obj["time"] = m_time;
        return obj;
    }

    static Course fromJson(const QJsonObject& obj) {
        return Course(
            obj["id"].toString(),
            obj["name"].toString(),
            obj["teacher"].toString(),
            obj["time"].toString()
        );
    }

    // getter/setter
    QString id() const { return m_id; }
    QString name() const { return m_name; }
    QString teacher() const { return m_teacher; }
    QString time() const { return m_time; }

private:
    QString m_id;       // 课程编号（唯一）
    QString m_name;     // 课程名称（如：瑜伽、力量训练）
    QString m_teacher;  // 教练
    QString m_time;     // 上课时间（如：周一19:00-20:30）
};

#endif // COURSE_H
