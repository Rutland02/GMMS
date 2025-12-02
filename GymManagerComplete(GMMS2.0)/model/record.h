#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QDateTime>

struct Record {
    QString memberId;
    QString memberName;
    QString courseName;
    QDateTime time;
    QString type;
};

#endif // RECORD_H
