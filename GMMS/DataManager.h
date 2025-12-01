#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include <QCoreApplication>
#include "Member.h"
#include "Course.h"
#include "CheckIn.h"       // 新增
#include "Reservation.h"   // 新增

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
    QList<Member> searchMembers(const QString& keyword) const; // 新增查询功能

    // 课程管理
    QList<Course> getAllCourses() const;
    bool addCourse(const Course& course);
    bool editCourse(const QString& oldId, const Course& newCourse);
    bool deleteCourse(const QString& id);
    Course getCourseById(const QString& id) const;

    // 签到管理（新增）
    QList<CheckIn> getAllCheckIns() const;
    bool addCheckIn(const CheckIn& checkIn);
    QList<CheckIn> getCheckInsByMemberId(const QString& memberId) const; // 按会员查询签到

    // 预约管理（新增）
    QList<Reservation> getAllReservations() const;
    bool addReservation(const Reservation& reservation);
    bool cancelReservation(const QString& memberId, const QString& courseId);
    bool isReserved(const QString& memberId, const QString& courseId) const;

private:
    DataManager(QObject *parent = nullptr);
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    const QString DATA_FILE_PATH = QCoreApplication::applicationDirPath() + QStringLiteral("/gym_data.json");

    // 数据存储（新增签到和预约列表）
    QList<Member> m_members;
    QList<Course> m_courses;
    QList<CheckIn> m_checkIns;       // 新增
    QList<Reservation> m_reservations; // 新增

    static DataManager* m_instance;
};

#endif // DATAMANAGER_H
