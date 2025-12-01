#include "CheckIn.h"

CheckIn::CheckIn(const QString& memberId, const QString& courseId, const QDateTime& checkInTime)
    : m_memberId(memberId), m_courseId(courseId), m_checkInTime(checkInTime) {}

QJsonObject CheckIn::toJson() const {
    QJsonObject json;
    json[QStringLiteral("memberId")] = m_memberId;
    json[QStringLiteral("courseId")] = m_courseId;
    json[QStringLiteral("checkInTime")] = m_checkInTime.toString(Qt::ISODate);
    return json;
}

CheckIn CheckIn::fromJson(const QJsonObject& obj) {
    CheckIn checkIn;
    checkIn.setMemberId(obj[QStringLiteral("memberId")].toString());
    checkIn.setCourseId(obj[QStringLiteral("courseId")].toString());
    checkIn.setCheckInTime(QDateTime::fromString(
        obj[QStringLiteral("checkInTime")].toString(), Qt::ISODate));
    return checkIn;
}

// Getter
QString CheckIn::memberId() const { return m_memberId; }
QString CheckIn::courseId() const { return m_courseId; }
QDateTime CheckIn::checkInTime() const { return m_checkInTime; }

// Setter
void CheckIn::setMemberId(const QString& memberId) { m_memberId = memberId; }
void CheckIn::setCourseId(const QString& courseId) { m_courseId = courseId; }
void CheckIn::setCheckInTime(const QDateTime& checkInTime) { m_checkInTime = checkInTime; }
