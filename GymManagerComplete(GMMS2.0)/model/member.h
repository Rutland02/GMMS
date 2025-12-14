#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QDate>

class Member {
public:
    Member();
    Member(const QString& cardId, const QString& name, const QDate& expiryDate, const QString& level, int points = 0);
    
    QString cardId() const;
    void setCardId(const QString& cardId);
    
    QString name() const;
    void setName(const QString& name);
    
    QDate expiryDate() const;
    void setExpiryDate(const QDate& expiryDate);
    
    QString level() const;
    void setLevel(const QString& level);
    
    int points() const;
    void setPoints(int points);
    
    void addPoints(int points);
    void reducePoints(int points);
    void checkLevelUpgrade();
    
    int getLevelWeight() const;
    bool isExpired() const;

private:
    QString m_cardId;
    QString m_name;
    QDate m_expiryDate;
    QString m_level;
    int m_points;
};

#endif // MEMBER_H
