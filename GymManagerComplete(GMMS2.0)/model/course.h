#ifndef COURSE_H
#define COURSE_H

#include <QString>

struct Course {
    QString id;
    QString name;
    QString coach;
    QString timeStr;
    int maxParticipants;
    int currentBooked;
};

#endif // COURSE_H
