#ifndef CHECKIN_H
#define CHECKIN_H
#include <QString>
#include <QDateTime>

class CheckIn {
public:
    CheckIn();
    CheckIn(const QString& memberId, const QString& memberName, const QString& courseId,
            const QString& courseName, const QDateTime& time, const QString& type);
    // 兼容早期数据：旧记录没有课程 ID。
    CheckIn(const QString& memberId, const QString& memberName, const QString& courseName, const QDateTime& time, const QString& type);
    
    QString memberId() const;
    void setMemberId(const QString& memberId);
    
    QString memberName() const;
    void setMemberName(const QString& memberName);

    QString courseId() const;
    void setCourseId(const QString& courseId);
    
    QString courseName() const;
    void setCourseName(const QString& courseName);
    
    QDateTime time() const;
    void setTime(const QDateTime& time);
    
    QString type() const;
    void setType(const QString& type);

private:
    QString m_memberId;
    QString m_memberName;
    QString m_courseId;
    QString m_courseName;
    QDateTime m_time;
    QString m_type;
};

#endif // CHECKIN_H
