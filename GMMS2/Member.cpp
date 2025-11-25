#include "Member.h"

// 带参构造函数实现
Member::Member(const QString& id, const QString& name, const QString& phone, const QString& joinDate)
    : m_id(id), m_name(name), m_phone(phone), m_joinDate(joinDate) {}

// JSON序列化
QJsonObject Member::toJson() const {
    QJsonObject json;
    json[QStringLiteral("id")] = m_id;
    json[QStringLiteral("name")] = m_name;
    json[QStringLiteral("phone")] = m_phone;
    json[QStringLiteral("joinDate")] = m_joinDate;
    return json;
}

// JSON反序列化
Member Member::fromJson(const QJsonObject& json) {
    Member member;
    member.setId(json[QStringLiteral("id")].toString());
    member.setName(json[QStringLiteral("name")].toString());
    member.setPhone(json[QStringLiteral("phone")].toString());
    member.setJoinDate(json[QStringLiteral("joinDate")].toString());
    return member;
}

// Getter实现
QString Member::id() const { return m_id; }
QString Member::name() const { return m_name; }
QString Member::phone() const { return m_phone; }
QString Member::joinDate() const { return m_joinDate; }

// Setter实现
void Member::setId(const QString &id) { m_id = id; }
void Member::setName(const QString &name) { m_name = name; }
void Member::setPhone(const QString &phone) { m_phone = phone; }
void Member::setJoinDate(const QString &joinDate) { m_joinDate = joinDate; }
