#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QDate>

struct Member {
    QString cardId;
    QString name;
    QDate expiryDate;
    QString level;

    int getLevelWeight() const {
        if (level == "钻石") return 4;
        if (level == "黄金") return 3;
        if (level == "白银") return 2;
        return 1;
    }
};

#endif // MEMBER_H
