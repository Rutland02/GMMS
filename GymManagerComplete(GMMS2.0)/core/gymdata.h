#ifndef GYMDATA_H
#define GYMDATA_H

#include <QObject>
#include <QVector>
#include "model/member.h"
#include "model/course.h"
#include "model/record.h"

class GymData : public QObject {
    Q_OBJECT
public:
    explicit GymData(QObject *parent = nullptr);

    // data access
    const QVector<Member>& getMembers() const;
    const QVector<Course>& getCourses() const;
    const QVector<Record>& getRecords() const;

    // operations
    bool addMember(const Member &m);
    bool addCourse(const Course &c);

    // booking: returns true on success, false on failure (and sets errorMsg)
    bool bookCourse(int memberIndex, int courseIndex, QString &errorMsg);
    bool checkIn(int memberIndex, int courseIndex, QString &errorMsg);

signals:
    void dataChanged(); // emitted when members/courses/records change

private:
    QVector<Member> members;
    QVector<Course> courses;
    QVector<Record> records;
};

#endif // GYMDATA_H
