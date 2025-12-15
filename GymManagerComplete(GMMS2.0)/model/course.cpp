#include "course.h"
#include <QStringList>
#include <Qt>

Course::Course() : m_maxParticipants(0), m_currentBooked(0), m_price(0.0), m_dayOfWeek(-1) {}

Course::Course(const QString& id, const QString& name, const QString& courseType, const QString& description, const QString& coach, 
               const QString& timeStr, double price, int maxParticipants, int currentBooked)
    : m_id(id), m_name(name), m_courseType(courseType), m_description(description), m_coach(coach), 
      m_timeStr(timeStr), m_price(price), m_maxParticipants(maxParticipants), m_currentBooked(currentBooked), m_dayOfWeek(-1) {
    parseTimeStr(timeStr);
}

QString Course::id() const {
    return m_id;
}

void Course::setId(const QString& id) {
    m_id = id;
}

QString Course::name() const {
    return m_name;
}

void Course::setName(const QString& name) {
    m_name = name;
}

QString Course::courseType() const {
    return m_courseType;
}

void Course::setCourseType(const QString& courseType) {
    m_courseType = courseType;
}

QString Course::description() const {
    return m_description;
}

void Course::setDescription(const QString& description) {
    m_description = description;
}

QString Course::coach() const {
    return m_coach;
}

void Course::setCoach(const QString& coach) {
    m_coach = coach;
}

QString Course::timeStr() const {
    return m_timeStr;
}

void Course::setTimeStr(const QString& timeStr) {
    m_timeStr = timeStr;
    parseTimeStr(timeStr);
}

int Course::dayOfWeek() const {
    return m_dayOfWeek;
}

void Course::setDayOfWeek(int day) {
    m_dayOfWeek = day;
    // 更新时间字符串
    QString dayStr;
    switch(day) {
        case 0: dayStr = "周一"; break;
        case 1: dayStr = "周二"; break;
        case 2: dayStr = "周三"; break;
        case 3: dayStr = "周四"; break;
        case 4: dayStr = "周五"; break;
        case 5: dayStr = "周六"; break;
        case 6: dayStr = "周日"; break;
        default: dayStr = "周一";
    }
    m_timeStr = QString("%1 %2").arg(dayStr).arg(m_timeSlot);
}

QString Course::timeSlot() const {
    return m_timeSlot;
}

void Course::setTimeSlot(const QString& timeSlot) {
    m_timeSlot = timeSlot;
    // 更新时间字符串
    QString dayStr;
    switch(m_dayOfWeek) {
        case 0: dayStr = "周一"; break;
        case 1: dayStr = "周二"; break;
        case 2: dayStr = "周三"; break;
        case 3: dayStr = "周四"; break;
        case 4: dayStr = "周五"; break;
        case 5: dayStr = "周六"; break;
        case 6: dayStr = "周日"; break;
        default: dayStr = "周一";
    }
    m_timeStr = QString("%1 %2").arg(dayStr).arg(timeSlot);
}

double Course::price() const {
    return m_price;
}

void Course::setPrice(double price) {
    m_price = price;
}

int Course::maxParticipants() const {
    return m_maxParticipants;
}

void Course::setMaxParticipants(int maxParticipants) {
    m_maxParticipants = maxParticipants;
}

int Course::currentBooked() const {
    return m_currentBooked;
}

void Course::setCurrentBooked(int currentBooked) {
    m_currentBooked = currentBooked;
}

bool Course::isFull() const {
    return m_currentBooked >= m_maxParticipants;
}

void Course::parseTimeStr(const QString& timeStr) {
    // 解析格式为 "周一 10:00" 的时间字符串
    QStringList parts = timeStr.split(" ", Qt::SkipEmptyParts);
    if (parts.size() == 2) {
        QString dayPart = parts[0];
        QString timePart = parts[1];
        
        // 解析星期几
        if (dayPart == "周一") m_dayOfWeek = 0;
        else if (dayPart == "周二") m_dayOfWeek = 1;
        else if (dayPart == "周三") m_dayOfWeek = 2;
        else if (dayPart == "周四") m_dayOfWeek = 3;
        else if (dayPart == "周五") m_dayOfWeek = 4;
        else if (dayPart == "周六") m_dayOfWeek = 5;
        else if (dayPart == "周日") m_dayOfWeek = 6;
        else m_dayOfWeek = -1;
        
        m_timeSlot = timePart;
    } else {
        m_dayOfWeek = -1;
        m_timeSlot = "";
    }
}
