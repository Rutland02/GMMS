#include "member.h"
#include <QDate>

Member::Member() : m_points(0) {
}

Member::Member(const QString& cardId, const QString& name, const QDate& expiryDate, const QString& level, int points)
    : m_cardId(cardId), m_name(name), m_expiryDate(expiryDate), m_level(level), m_points(points) {
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

int Member::points() const {
    return m_points;
}

void Member::setPoints(int points) {
    m_points = points;
    checkLevelUpgrade();
}

void Member::addPoints(int points) {
    m_points += points;
    checkLevelUpgrade();
}

void Member::reducePoints(int points) {
    m_points -= points;
    if (m_points < 0) {
        m_points = 0;
    }
    checkLevelUpgrade();
}

void Member::checkLevelUpgrade() {
    // 根据积分自动升级等级
    if (m_points >= 5000) {
        m_level = "钻石";
    } else if (m_points >= 2000) {
        m_level = "黄金";
    } else if (m_points >= 500) {
        m_level = "白银";
    } else {
        m_level = "普通";
    }
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
