#include "checkin.h"


CheckIn::CheckIn() {}

CheckIn::CheckIn(const QString& memberId, const QString& memberName, const QString& courseName, const QDateTime& time, const QString& type)
    : m_memberId(memberId), m_memberName(memberName), m_courseName(courseName), m_time(time), m_type(type) {}

QString CheckIn::memberId() const {
    return m_memberId;
}

void CheckIn::setMemberId(const QString& memberId) {
    m_memberId = memberId;
}

QString CheckIn::memberName() const {
    return m_memberName;
}

void CheckIn::setMemberName(const QString& memberName) {
    m_memberName = memberName;
}

QString CheckIn::courseName() const {
    return m_courseName;
}

void CheckIn::setCourseName(const QString& courseName) {
    m_courseName = courseName;
}

QDateTime CheckIn::time() const {
    return m_time;
}

void CheckIn::setTime(const QDateTime& time) {
    m_time = time;
}

QString CheckIn::type() const {
    return m_type;
}

void CheckIn::setType(const QString& type) {
    m_type = type;
}
