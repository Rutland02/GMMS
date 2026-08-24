#include "gymdata.h"
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QSaveFile>
#include <QRegularExpression>

GymData::GymData(QObject *parent) : QObject(parent)
{
    members.push_back(Member("1001", "张三", QDate::currentDate().addYears(1), "黄金"));
    members.push_back(Member("1002", "李四", QDate::currentDate().addMonths(6), "普通"));
    members.push_back(Member("1003", "王五", QDate::currentDate().addYears(2), "钻石"));

    courses.push_back(Course("C01", "瑜伽入门", "瑜伽", "基础瑜伽课程，适合初学者", "Lisa", "周一 10:00", 68.0, 10, 0));
    courses.push_back(Course("C02", "高强度间歇(HIIT)", "力量训练", "高强度间歇训练，提高心肺功能", "Mike", "周三 18:30", 88.0, 5, 0));
    courses.push_back(Course("C03", "动感单车", "有氧训练", "动感单车课程，燃烧脂肪", "Sara", "周五 19:00", 78.0, 20, 0));
}

const QVector<Member>& GymData::getMembers() const {
    return members;
}

const QVector<Course>& GymData::getCourses() const {
    return courses;
}

const QVector<CheckIn>& GymData::getCheckIns() const {
    return checkins;
}

bool GymData::addMember(const Member &m) {
    for (const auto &x : members)
        if (x.cardId() == m.cardId())
            return false;

    members.push_back(m);
    emit dataChanged();
    return true;
}

int GymData::findMemberIndex(const QString &cardId) const {
    for (int i = 0; i < members.size(); ++i) {
        if (members[i].cardId() == cardId) {
            return i;
        }
    }
    return -1;
}

int GymData::findCourseIndex(const QString &courseId) const {
    for (int i = 0; i < courses.size(); ++i) {
        if (courses[i].id() == courseId) {
            return i;
        }
    }
    return -1;
}

QString GymData::nextCourseId() const {
    int maxId = 0;
    static const QRegularExpression pattern("^C(\\d+)$");
    for (const auto &course : courses) {
        const QRegularExpressionMatch match = pattern.match(course.id());
        if (match.hasMatch()) {
            maxId = qMax(maxId, match.captured(1).toInt());
        }
    }
    return QString("C%1").arg(maxId + 1, 2, 10, QChar('0'));
}

bool GymData::editMember(const QString &cardId, const Member &newMember) {
    const int index = findMemberIndex(cardId);
    if (index < 0)
        return false;
    
    // 如果修改了卡号，需要检查新卡号是否已存在
    if (members[index].cardId() != newMember.cardId()) {
        for (const auto &x : members)
            if (x.cardId() == newMember.cardId())
                return false;
    }
    
    members[index] = newMember;
    if (cardId != newMember.cardId()) {
        for (auto &checkin : checkins) {
            if (checkin.memberId() == cardId) {
                checkin.setMemberId(newMember.cardId());
            }
        }
    }
    emit dataChanged();
    return true;
}

bool GymData::deleteMember(const QString &cardId) {
    const int index = findMemberIndex(cardId);
    if (index < 0)
        return false;
    
    members.removeAt(index);
    emit dataChanged();
    return true;
}

bool GymData::addCourse(const Course &c) {
    for (const auto &x : courses)
        if (x.id() == c.id())
            return false;

    courses.push_back(c);
    emit dataChanged();
    return true;
}

bool GymData::editCourse(const QString &courseId, const Course &newCourse) {
    const int index = findCourseIndex(courseId);
    if (index < 0)
        return false;
    
    // 如果修改了课程ID，需要检查新ID是否已存在
    if (courses[index].id() != newCourse.id()) {
        for (const auto &x : courses)
            if (x.id() == newCourse.id())
                return false;
    }
    
    courses[index] = newCourse;
    if (courseId != newCourse.id()) {
        for (auto &checkin : checkins) {
            if (checkin.courseId() == courseId) {
                checkin.setCourseId(newCourse.id());
            }
        }
    }
    emit dataChanged();
    return true;
}

bool GymData::deleteCourse(const QString &courseId) {
    const int index = findCourseIndex(courseId);
    if (index < 0)
        return false;
    
    courses.removeAt(index);
    emit dataChanged();
    return true;
}

bool GymData::bookCourse(const QString &memberId, const QString &courseId, QString &err) {
    const int mi = findMemberIndex(memberId);
    const int ci = findCourseIndex(courseId);
    if (mi < 0) {
        err = "无效的会员卡号";
        return false;
    }
    if (ci < 0) {
        err = "无效的课程 ID";
        return false;
    }

    auto &m = members[mi];
    Course &c = courses[ci];

    if (m.isExpired()) {
        err = "会员卡已过期";
        return false;
    }

    // 检查课程是否已过期
    if (c.isExpired()) {
        err = "该课程已过期";
        return false;
    }

    // 检查是否已预约该课程
    for (const auto &checkin : checkins) {
        if (checkin.memberId() == m.cardId() &&
            (checkin.courseId() == c.id() ||
             (checkin.courseId().isEmpty() && checkin.courseName() == c.name())) &&
            checkin.type() == "预约") {
            err = "该会员已预约此课程";
            return false;
        }
    }

    // 预约容量是课程的硬上限，会员等级只影响个人预约数量。
    if (c.currentBooked() >= c.maxParticipants()) {
        err = "课程人数已满";
        return false;
    }

    // 根据会员等级限制可预约课程数量
    int bookedCount = 0;
    for (const auto &checkin : checkins) {
        if (checkin.memberId() == m.cardId() && checkin.type() == "预约") {
            bookedCount++;
        }
    }
    
    if (m.level() == "普通" && bookedCount >= 2) {
        err = "普通会员最多可同时预约2门课程";
        return false;
    } else if (m.level() == "黄金" && bookedCount >= 3) {
        err = "黄金会员最多可同时预约3门课程";
        return false;
    } else if (m.level() == "钻石" && bookedCount >= 5) {
        err = "钻石会员最多可同时预约5门课程";
        return false;
    }

    c.setCurrentBooked(c.currentBooked() + 1);

    // 预约课程奖励积分
    m.addPoints(5);

    checkins.push_front(CheckIn(
        m.cardId(),
        m.name(),
        c.id(),
        c.name(),
        QDateTime::currentDateTime(),
        "预约"
    ));

    emit dataChanged();
    return true;
}

bool GymData::checkIn(const QString &memberId, const QString &courseId, QString &err) {
    const int mi = findMemberIndex(memberId);
    const int ci = findCourseIndex(courseId);
    if (mi < 0) {
        err = "无效的会员卡号";
        return false;
    }
    if (ci < 0) {
        err = "无效的课程 ID";
        return false;
    }

    auto &m = members[mi];
    const Course &c = courses[ci];

    if (m.isExpired()) {
        err = "会员卡已过期";
        return false;
    }

    // 检查课程是否已过期
    if (c.isExpired()) {
        err = "该课程已过期";
        return false;
    }

    // 检查是否已签到该课程
    for (const auto &checkin : checkins) {
        if (checkin.memberId() == m.cardId() &&
            (checkin.courseId() == c.id() ||
             (checkin.courseId().isEmpty() && checkin.courseName() == c.name())) &&
            checkin.type() == "签到") {
            err = "该会员已签到此课程";
            return false;
        }
    }

    // 检查是否已预约该课程（钻石会员可直接签到，无需预约）
    bool hasBooked = m.level() == "钻石";
    if (!hasBooked) {
        for (const auto &checkin : checkins) {
            if (checkin.memberId() == m.cardId() &&
                (checkin.courseId() == c.id() ||
                 (checkin.courseId().isEmpty() && checkin.courseName() == c.name())) &&
                checkin.type() == "预约") {
                hasBooked = true;
                break;
            }
        }
    }

    if (!hasBooked) {
        err = "该会员未预约此课程，无法签到";
        return false;
    }

    // 根据会员等级限制签到次数
    int checkInCount = 0;
    QDate today = QDate::currentDate();
    for (const auto &checkin : checkins) {
        if (checkin.memberId() == m.cardId() && 
            checkin.type() == "签到" && 
            checkin.time().date() == today) {
            checkInCount++;
        }
    }
    
    if (m.level() == "普通" && checkInCount >= 2) {
        err = "普通会员每天最多签到2次";
        return false;
    } else if (m.level() == "黄金" && checkInCount >= 3) {
        err = "黄金会员每天最多签到3次";
        return false;
    }
    // 钻石会员无限制

    // 签到奖励积分
    m.addPoints(10);

    checkins.push_front(CheckIn(
        m.cardId(),
        m.name(),
        c.id(),
        c.name(),
        QDateTime::currentDateTime(),
        "签到"
    ));

    emit dataChanged();
    return true;
}

bool GymData::saveToJson(const QString &filePath) {
    QJsonObject json;
    
    // 保存会员数据
    QJsonArray membersArray;
    for (const auto &member : members) {
        QJsonObject memberObj;
        memberObj["cardId"] = member.cardId();
        memberObj["name"] = member.name();
        memberObj["expiryDate"] = member.expiryDate().toString("yyyy-MM-dd");
        memberObj["level"] = member.level();
        memberObj["points"] = member.points();
        membersArray.append(memberObj);
    }
    json["members"] = membersArray;
    
    // 保存课程数据
    QJsonArray coursesArray;
    for (const auto &course : courses) {
        QJsonObject courseObj;
        courseObj["id"] = course.id();
        courseObj["name"] = course.name();
        courseObj["courseType"] = course.courseType();
        courseObj["description"] = course.description();
        courseObj["coach"] = course.coach();
        courseObj["timeStr"] = course.timeStr();
        courseObj["price"] = course.price();
        courseObj["maxParticipants"] = course.maxParticipants();
        courseObj["currentBooked"] = course.currentBooked();
        courseObj["startDate"] = course.startDate().toString("yyyy-MM-dd");
        courseObj["endDate"] = course.endDate().toString("yyyy-MM-dd");
        coursesArray.append(courseObj);
    }
    json["courses"] = coursesArray;
    
    // 保存签到记录
    QJsonArray checkinsArray;
    for (const auto &checkin : checkins) {
        QJsonObject checkinObj;
        checkinObj["memberId"] = checkin.memberId();
        checkinObj["memberName"] = checkin.memberName();
        checkinObj["courseId"] = checkin.courseId();
        checkinObj["courseName"] = checkin.courseName();
        checkinObj["time"] = checkin.time().toString("yyyy-MM-dd HH:mm:ss");
        checkinObj["type"] = checkin.type();
        checkinsArray.append(checkinObj);
    }
    json["checkins"] = checkinsArray;
    
    // 原子写入：写入失败时不会直接截断原文件。
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QJsonDocument doc(json);
    if (file.write(doc.toJson(QJsonDocument::Indented)) < 0) {
        file.cancelWriting();
        return false;
    }
    return file.commit();
}

bool GymData::loadFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return false;
    }
    
    QJsonObject json = doc.object();
    
    if (!json.value("members").isArray() ||
        !json.value("courses").isArray() ||
        !json.value("checkins").isArray()) {
        return false;
    }

    QVector<Member> loadedMembers;
    QVector<Course> loadedCourses;
    QVector<CheckIn> loadedCheckIns;

    // 加载会员数据到临时容器，全部成功后再替换当前数据。
    QJsonArray membersArray = json["members"].toArray();
    for (const auto &memberValue : membersArray) {
        if (!memberValue.isObject()) {
            return false;
        }
        QJsonObject memberObj = memberValue.toObject();
        QString cardId = memberObj["cardId"].toString();
        QString name = memberObj["name"].toString();
        QDate expiryDate = QDate::fromString(memberObj["expiryDate"].toString(), "yyyy-MM-dd");
        QString level = memberObj["level"].toString();
        int points = memberObj["points"].toInt();
        if (cardId.isEmpty() || name.isEmpty() || !expiryDate.isValid() || points < 0) {
            return false;
        }
        for (const auto &member : loadedMembers) {
            if (member.cardId() == cardId) {
                return false;
            }
        }
        loadedMembers.push_back(Member(cardId, name, expiryDate, level, points));
    }
    
    // 加载课程数据
    QJsonArray coursesArray = json["courses"].toArray();
    for (const auto &courseValue : coursesArray) {
        if (!courseValue.isObject()) {
            return false;
        }
        QJsonObject courseObj = courseValue.toObject();
        QString id = courseObj["id"].toString();
        QString name = courseObj["name"].toString();
        QString courseType = courseObj["courseType"].toString();
        QString description = courseObj["description"].toString();
        QString coach = courseObj["coach"].toString();
        QString timeStr = courseObj["timeStr"].toString();
        double price = courseObj["price"].toDouble();
        int maxParticipants = courseObj["maxParticipants"].toInt();
        int currentBooked = courseObj["currentBooked"].toInt();
        QDate startDate = QDate::fromString(courseObj["startDate"].toString(), "yyyy-MM-dd");
        QDate endDate = QDate::fromString(courseObj["endDate"].toString(), "yyyy-MM-dd");
        if (id.isEmpty() || name.isEmpty() || !startDate.isValid() || !endDate.isValid() ||
            startDate > endDate || price < 0 || maxParticipants <= 0 ||
            currentBooked < 0 || currentBooked > maxParticipants) {
            return false;
        }
        for (const auto &course : loadedCourses) {
            if (course.id() == id) {
                return false;
            }
        }
        loadedCourses.push_back(Course(id, name, courseType, description, coach, timeStr,
                                       price, maxParticipants, currentBooked, startDate, endDate));
    }
    
    // 加载签到记录
    QJsonArray checkinsArray = json["checkins"].toArray();
    for (const auto &checkinValue : checkinsArray) {
        if (!checkinValue.isObject()) {
            return false;
        }
        QJsonObject checkinObj = checkinValue.toObject();
        QString memberId = checkinObj["memberId"].toString();
        QString memberName = checkinObj["memberName"].toString();
        QString courseId = checkinObj["courseId"].toString();
        QString courseName = checkinObj["courseName"].toString();
        QDateTime time = QDateTime::fromString(checkinObj["time"].toString(), "yyyy-MM-dd HH:mm:ss");
        QString type = checkinObj["type"].toString();
        if (memberId.isEmpty() || memberName.isEmpty() || courseName.isEmpty() ||
            !time.isValid() || (type != "预约" && type != "签到")) {
            return false;
        }
        loadedCheckIns.push_back(CheckIn(memberId, memberName, courseId, courseName, time, type));
    }

    members = loadedMembers;
    courses = loadedCourses;
    checkins = loadedCheckIns;
    
    emit dataChanged();
    return true;
}

// 统计分析方法实现
int GymData::getTotalMembers() const {
    return members.size();
}

int GymData::getActiveMembers() const {
    int count = 0;
    for (const auto &member : members) {
        if (!member.isExpired()) {
            count++;
        }
    }
    return count;
}

int GymData::getExpiredMembers() const {
    int count = 0;
    for (const auto &member : members) {
        if (member.isExpired()) {
            count++;
        }
    }
    return count;
}

int GymData::getTotalPoints() const {
    int total = 0;
    for (const auto &member : members) {
        total += member.points();
    }
    return total;
}

double GymData::getAveragePoints() const {
    if (members.isEmpty()) {
        return 0.0;
    }
    return static_cast<double>(getTotalPoints()) / members.size();
}

QMap<QString, int> GymData::getMembersByLevel() const {
    QMap<QString, int> result;
    for (const auto &member : members) {
        result[member.level()]++;
    }
    return result;
}

int GymData::getTotalCourses() const {
    return courses.size();
}

int GymData::getAvailableCourses() const {
    int count = 0;
    for (const auto &course : courses) {
        if (!course.isExpired() && course.currentBooked() < course.maxParticipants()) {
            count++;
        }
    }
    return count;
}

int GymData::getTotalBookings() const {
    int count = 0;
    for (const auto &checkin : checkins) {
        if (checkin.type() == "预约") {
            count++;
        }
    }
    return count;
}

int GymData::getTotalCheckIns() const {
    int count = 0;
    for (const auto &checkin : checkins) {
        if (checkin.type() == "签到") {
            count++;
        }
    }
    return count;
}

QMap<QString, int> GymData::getBookingsByCourse() const {
    QMap<QString, int> result;
    for (const auto &checkin : checkins) {
        if (checkin.type() == "预约") {
            result[checkin.courseName()]++;
        }
    }
    return result;
}

QMap<QString, int> GymData::getCheckInsByCourse() const {
    QMap<QString, int> result;
    for (const auto &checkin : checkins) {
        if (checkin.type() == "签到") {
            result[checkin.courseName()]++;
        }
    }
    return result;
}

QMap<QDate, int> GymData::getDailyActivity(const QDate &startDate, const QDate &endDate) const {
    QMap<QDate, int> result;
    
    // 初始化日期范围
    QDate currentDate = startDate;
    while (currentDate <= endDate) {
        result[currentDate] = 0;
        currentDate = currentDate.addDays(1);
    }
    
    // 统计每日活动
    for (const auto &checkin : checkins) {
        QDate checkinDate = checkin.time().date();
        if (checkinDate >= startDate && checkinDate <= endDate) {
            result[checkinDate]++;
        }
    }
    
    return result;
}
