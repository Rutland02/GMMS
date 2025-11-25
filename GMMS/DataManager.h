#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include "Member.h"
#include "Course.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    // 会员数据操作
    QList<Member> getAllMembers();          // 获取所有会员
    bool addMember(const Member &member);   // 添加会员（返回是否成功）
    bool editMember(const QString &id, const Member &newMember); // 编辑会员
    bool deleteMember(const QString &id);   // 删除会员
    Member getMemberById(const QString &id); // 根据ID获取会员
    bool isMemberIdExists(const QString &id); // 检查会员ID是否已存在

    // 课程数据操作
    QList<Course> getAllCourses();          // 获取所有课程
    bool addCourse(const Course &course);   // 添加课程（返回是否成功）
    bool editCourse(const QString &id, const Course &newCourse); // 编辑课程
    bool deleteCourse(const QString &id);   // 删除课程
    Course getCourseById(const QString &id); // 根据ID获取课程
    bool isCourseIdExists(const QString &id); // 检查课程ID是否已存在

private:
    // 数据文件路径（会员+课程共用一个JSON文件）
    const QString DATA_FILE_PATH = QCoreApplication::applicationDirPath() + "/gym_data.json";

    // 读取JSON文件到内存
    bool loadData();
    // 将内存数据写入JSON文件
    bool saveData();

    // 内存中的数据存储
    QList<Member> m_members;
    QList<Course> m_courses;
};

#endif // DATAMANAGER_H
