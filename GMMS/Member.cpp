#include "Member.h"

Member::Member()
    : m_id(QString()), m_name(QString()), m_phone(QString()), m_joinDate(QString())
{
}

Member::Member(const QString &id, const QString &name, const QString &phone, const QString &joinDate)
    : m_id(id), m_name(name), m_phone(phone), m_joinDate(joinDate)
{
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

// 转换为JSON对象
QJsonObject Member::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["phone"] = m_phone;
    json["joinDate"] = m_joinDate;
    return json;
}

// 从JSON对象解析
Member Member::fromJson(const QJsonObject &json)
{
    Member member;
    member.setId(json["id"].toString());
    member.setName(json["name"].toString());
    member.setPhone(json["phone"].toString());
    member.setJoinDate(json["joinDate"].toString());
    return member;
}
