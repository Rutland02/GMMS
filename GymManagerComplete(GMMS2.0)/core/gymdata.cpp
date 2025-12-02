#include "gymdata.h"
#include <QDate>

GymData::GymData(QObject *parent)
    : QObject(parent)
{
    // init mock data
    members.push_back({"1001", "张三", QDate::currentDate().addYears(1), "黄金"});
    members.push_back({"1002", "李四", QDate::currentDate().addMonths(6), "普通"});
    members.push_back({"1003", "王五", QDate::currentDate().addYears(2), "钻石"});

    courses.push_back({"C01", "瑜伽入门", "Lisa", "周一 10:00", 10, 0});
    courses.push_back({"C02", "高强度间歇(HIIT)", "Mike", "周三 18:30", 5, 0});
    courses.push_back({"C03", "动感单车", "Sara", "周五 19:00", 20, 0});
}

const QVector<Member>& GymData::getMembers() const { return members; }
const QVector<Course>& GymData::getCourses() const { return courses; }
const QVector<Record>& GymData::getRecords() const { return records; }

bool GymData::addMember(const Member &m) {
    // check duplicate cardId
    for (const auto &mm : members) {
        if (mm.cardId == m.cardId) return false;
    }
    members.push_back(m);
    emit dataChanged();
    return true;
}

bool GymData::addCourse(const Course &c) {
    // simple id uniqueness check
    for (const auto &cc : courses) {
        if (cc.id == c.id) return false;
    }
    courses.push_back(c);
    emit dataChanged();
    return true;
}

bool GymData::bookCourse(int memberIndex, int courseIndex, QString &errorMsg) {
    if (memberIndex < 0 || memberIndex >= members.size()) {
        errorMsg = "无效的会员索引";
        return false;
    }
    if (courseIndex < 0 || courseIndex >= courses.size()) {
        errorMsg = "无效的课程索引";
        return false;
    }
    Member &m = members[memberIndex];
    Course &c = courses[courseIndex];

    if (m.expiryDate < QDate::currentDate()) {
        errorMsg = "会员卡已过期";
        return false;
    }

    if (c.currentBooked >= c.maxParticipants) {
        // allow diamond one extra
        if (m.level == "钻石" && c.currentBooked < c.maxParticipants + 1) {
            // allow
        } else {
            errorMsg = "课程人数已满";
            return false;
        }
    }

    c.currentBooked++;
    records.push_front({m.cardId, m.name, c.name, QDateTime::currentDateTime(), "预约"});
    emit dataChanged();
    return true;
}

bool GymData::checkIn(int memberIndex, int courseIndex, QString &errorMsg) {
    if (memberIndex < 0 || memberIndex >= members.size()) {
        errorMsg = "无效的会员索引";
        return false;
    }
    if (courseIndex < 0 || courseIndex >= courses.size()) {
        errorMsg = "无效的课程索引";
        return false;
    }
    Member &m = members[memberIndex];
    Course &c = courses[courseIndex];

    if (m.expiryDate < QDate::currentDate()) {
        errorMsg = "会员卡已过期";
        return false;
    }

    // For check-in we simply record it; in real app you'd validate booking.
    records.push_front({m.cardId, m.name, c.name, QDateTime::currentDateTime(), "签到"});
    emit dataChanged();
    return true;
}
