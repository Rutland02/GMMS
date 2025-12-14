#ifndef COURSE_H
#define COURSE_H

#include <QString>

class Course {
public:
    Course();
    Course(const QString& id, const QString& name, const QString& coach, const QString& timeStr, int maxParticipants, int currentBooked);
    
    QString id() const;
    void setId(const QString& id);
    
    QString name() const;
    void setName(const QString& name);
    
    QString coach() const;
    void setCoach(const QString& coach);
    
    QString timeStr() const;
    void setTimeStr(const QString& timeStr);
    
    int maxParticipants() const;
    void setMaxParticipants(int maxParticipants);
    
    int currentBooked() const;
    void setCurrentBooked(int currentBooked);
    
    bool isFull() const;

private:
    QString m_id;
    QString m_name;
    QString m_coach;
    QString m_timeStr;
    int m_maxParticipants;
    int m_currentBooked;
};

#endif // COURSE_H
