#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QJsonObject>

class Course {
public:
    // 构造函数声明
    Course() = default;
    Course(const QString& id, const QString& name, const QString& teacher, const QString& time, int maxNum);

    // JSON序列化/反序列化
    QJsonObject toJson() const;
    static Course fromJson(const QJsonObject& obj);

    // Getter
    QString id() const;
    QString name() const;
    QString teacher() const;
    QString time() const;
    int maxNum() const;

    // Setter
    void setId(const QString &id);
    void setName(const QString &name);
    void setTeacher(const QString &teacher);
    void setTime(const QString &time);
    void setMaxNum(int maxNum);

private:
    QString m_id;         // 课程ID
    QString m_name;       // 课程名称
    QString m_teacher;    // 授课老师
    QString m_time;       // 上课时间
    int m_maxNum;         // 最大人数
};

#endif // COURSE_H
