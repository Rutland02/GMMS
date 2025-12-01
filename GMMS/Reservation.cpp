#include "Reservation.h"

Reservation::Reservation(const QString& memberId, const QString& courseId, const QDateTime& reserveTime)
    : m_memberId(memberId), m_courseId(courseId), m_reserveTime(reserveTime) {}

QJsonObject Reservation::toJson() const {
    QJsonObject json;
    json[QStringLiteral("memberId")] = m_memberId;
    json[QStringLiteral("courseId")] = m_courseId;
    json[QStringLiteral("reserveTime")] = m_reserveTime.toString(Qt::ISODate);
    return json;
}

Reservation Reservation::fromJson(const QJsonObject& obj) {
    Reservation reservation;
    reservation.setMemberId(obj[QStringLiteral("memberId")].toString());
    reservation.setCourseId(obj[QStringLiteral("courseId")].toString());
    reservation.setReserveTime(QDateTime::fromString(
        obj[QStringLiteral("reserveTime")].toString(), Qt::ISODate));
    return reservation;
}

// Getter
QString Reservation::memberId() const { return m_memberId; }
QString Reservation::courseId() const { return m_courseId; }
QDateTime Reservation::reserveTime() const { return m_reserveTime; }

// Setter
void Reservation::setMemberId(const QString& memberId) { m_memberId = memberId; }
void Reservation::setCourseId(const QString& courseId) { m_courseId = courseId; }
void Reservation::setReserveTime(const QDateTime& reserveTime) { m_reserveTime = reserveTime; }
