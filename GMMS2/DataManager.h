#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include <QCoreApplication>
#include "Member.h"
#include "Course.h"

class DataManager : public QObject {
    Q_OBJECT
public:
    // 单例模式
    static DataManager* getInstance();
    ~DataManager();

    // 数据加载/保存
    bool loadData();
    bool saveData();

    // 会员管理
    QList<Member> getAllMembers() const;
    bool addMember(const Member& member);
    bool editMember(const QString& oldId, const Member& newMember);
    bool deleteMember(const QString& id);
    Member getMemberById(const QString& id) const;

    // 课程管理
    QList<Course> getAllCourses() const;
    bool addCourse(const Course& course);
    bool editCourse(const QString& oldId, const Course& newCourse);
    bool deleteCourse(const QString& id);
    Course getCourseById(const QString& id) const;

private:
    // 私有构造（单例）
    DataManager(QObject *parent = nullptr);
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    // 数据文件路径
    const QString DATA_FILE_PATH = QCoreApplication::applicationDirPath() + QStringLiteral("/gym_data.json");

    // 数据存储
    QList<Member> m_members;
    QList<Course> m_courses;

    // 单例实例
    static DataManager* m_instance;
};

#endif // DATAMANAGER_H
