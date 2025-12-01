#ifndef CHECKIN_H
#define CHECKIN_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class CheckIn {
public:
    // 构造函数
    CheckIn() = default;
    CheckIn(const QString& memberId, const QString& courseId, const QDateTime& checkInTime);

    // JSON序列化/反序列化
    QJsonObject toJson() const;
    static CheckIn fromJson(const QJsonObject& obj);

    // Getter
    QString memberId() const;
    QString courseId() const;
    QDateTime checkInTime() const;

    // Setter
    void setMemberId(const QString& memberId);
    void setCourseId(const QString& courseId);
    void setCheckInTime(const QDateTime& checkInTime);

private:
    QString m_memberId;    // 会员ID
    QString m_courseId;    // 课程ID
    QDateTime m_checkInTime; // 签到时间
};

#endif // CHECKIN_H
