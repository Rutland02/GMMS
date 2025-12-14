#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QDate>

class Member {
public:
    Member();
    Member(const QString& cardId, const QString& name, const QDate& expiryDate, const QString& level);
    
    QString cardId() const;
    void setCardId(const QString& cardId);
    
    QString name() const;
    void setName(const QString& name);
    
    QDate expiryDate() const;
    void setExpiryDate(const QDate& expiryDate);
    
    QString level() const;
    void setLevel(const QString& level);
    
    int getLevelWeight() const;
    bool isExpired() const;

private:
    QString m_cardId;
    QString m_name;
    QDate m_expiryDate;
    QString m_level;
};

#endif // MEMBER_H
