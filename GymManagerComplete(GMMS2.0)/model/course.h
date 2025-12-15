#ifndef COURSE_H
#define COURSE_H

#include <QString>

class Course {
public:
    Course();
    Course(const QString& id, const QString& name, const QString& courseType, const QString& description, const QString& coach, 
           const QString& timeStr, double price, int maxParticipants, int currentBooked);
    
    QString id() const;
    void setId(const QString& id);
    
    QString name() const;
    void setName(const QString& name);
    
    QString courseType() const;
    void setCourseType(const QString& courseType);
    
    QString description() const;
    void setDescription(const QString& description);
    
    QString coach() const;
    void setCoach(const QString& coach);
    
    QString timeStr() const;
    void setTimeStr(const QString& timeStr);
    
    int dayOfWeek() const;
    void setDayOfWeek(int day);
    
    QString timeSlot() const;
    void setTimeSlot(const QString& timeSlot);
    
    double price() const;
    void setPrice(double price);
    
    int maxParticipants() const;
    void setMaxParticipants(int maxParticipants);
    
    int currentBooked() const;
    void setCurrentBooked(int currentBooked);
    
    bool isFull() const;

private:
    QString m_id;
    QString m_name;
    QString m_courseType;
    QString m_description;
    QString m_coach;
    QString m_timeStr;
    int m_dayOfWeek; // 0-6, 0表示周一，6表示周日
    QString m_timeSlot; // 时间段，如 "10:00"
    double m_price;
    int m_maxParticipants;
    int m_currentBooked;
    
    void parseTimeStr(const QString& timeStr);
};

#endif // COURSE_H
