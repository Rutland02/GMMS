#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QJsonObject>

class Member {
public:
    // 构造函数
    Member() = default;
    Member(const QString& id, const QString& name, const QString& phone, const QString& joinDate);

    // JSON序列化/反序列化
    QJsonObject toJson() const;
    static Member fromJson(const QJsonObject& obj);

    // Getter
    QString id() const;
    QString name() const;
    QString phone() const;
    QString joinDate() const;

    // Setter
    void setId(const QString &id);
    void setName(const QString &name);
    void setPhone(const QString &phone);
    void setJoinDate(const QString &joinDate);

private:
    QString m_id;         // 会员ID
    QString m_name;       // 会员姓名
    QString m_phone;      // 联系电话
    QString m_joinDate;   // 加入日期（YYYY-MM-DD）
};

#endif // MEMBER_H
