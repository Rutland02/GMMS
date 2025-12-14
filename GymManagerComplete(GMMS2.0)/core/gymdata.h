#ifndef GYMDATA_H
#define GYMDATA_H
#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDate>
#include "model/member.h"
#include "model/course.h"
#include "model/checkin.h"

class GymData: public QObject{
    Q_OBJECT
public:
    explicit GymData(QObject*parent=nullptr);
    const QVector<Member>& getMembers() const;
    const QVector<Course>& getCourses() const;
    const QVector<CheckIn>& getCheckIns() const;
    bool addMember(const Member&);
    bool editMember(int index, const Member& newMember);
    bool deleteMember(int index);
    bool addCourse(const Course&);
    bool bookCourse(int,int,QString&);
    bool checkIn(int,int,QString&);
    bool loadFromJson(const QString& filePath);
    bool saveToJson(const QString& filePath);
    
    // 统计分析方法
    int getTotalMembers() const;
    int getActiveMembers() const;
    int getExpiredMembers() const;
    QMap<QString, int> getMembersByLevel() const;
    
    int getTotalCourses() const;
    int getAvailableCourses() const;
    
    int getTotalBookings() const;
    int getTotalCheckIns() const;
    QMap<QString, int> getBookingsByCourse() const;
    QMap<QString, int> getCheckInsByCourse() const;
    QMap<QDate, int> getDailyActivity(const QDate& startDate, const QDate& endDate) const;
    
signals: void dataChanged();
private:
    QVector<Member> members;
    QVector<Course> courses;
    QVector<CheckIn> checkins;
};
#endif
