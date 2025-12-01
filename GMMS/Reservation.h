#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Reservation {
public:
    // 构造函数
    Reservation() = default;
    Reservation(const QString& memberId, const QString& courseId, const QDateTime& reserveTime);

    // JSON序列化/反序列化
    QJsonObject toJson() const;
    static Reservation fromJson(const QJsonObject& obj);

    // Getter
    QString memberId() const;
    QString courseId() const;
    QDateTime reserveTime() const;

    // Setter
    void setMemberId(const QString& memberId);
    void setCourseId(const QString& courseId);
    void setReserveTime(const QDateTime& reserveTime);

private:
    QString m_memberId;     // 会员ID
    QString m_courseId;     // 课程ID
    QDateTime m_reserveTime; // 预约时间
};

#endif // RESERVATION_H
