#include "course.h"

Course::Course() : m_maxParticipants(0), m_currentBooked(0) {}

Course::Course(const QString& id, const QString& name, const QString& coach, const QString& timeStr, int maxParticipants, int currentBooked)
    : m_id(id), m_name(name), m_coach(coach), m_timeStr(timeStr), m_maxParticipants(maxParticipants), m_currentBooked(currentBooked) {}

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
