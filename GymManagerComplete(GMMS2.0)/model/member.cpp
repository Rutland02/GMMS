#include "member.h"
#include <QDate>

Member::Member() {
}

Member::Member(const QString& cardId, const QString& name, const QDate& expiryDate, const QString& level)
    : m_cardId(cardId), m_name(name), m_expiryDate(expiryDate), m_level(level) {
}

QString Member::cardId() const {
    return m_cardId;
}

void Member::setCardId(const QString& cardId) {
    m_cardId = cardId;
}

QString Member::name() const {
    return m_name;
}

void Member::setName(const QString& name) {
    m_name = name;
}

QDate Member::expiryDate() const {
    return m_expiryDate;
}

void Member::setExpiryDate(const QDate& expiryDate) {
    m_expiryDate = expiryDate;
}

QString Member::level() const {
    return m_level;
}

void Member::setLevel(const QString& level) {
    m_level = level;
}

int Member::getLevelWeight() const {
    if (m_level == "钻石") return 4;
    if (m_level == "黄金") return 3;
    if (m_level == "白银") return 2;
    return 1;
}

bool Member::isExpired() const {
    return m_expiryDate < QDate::currentDate();
}
