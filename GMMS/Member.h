#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QJsonObject>

class Member
{
public:
    Member();
    Member(const QString &id, const QString &name, const QString &phone, const QString &joinDate);

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

    // 转换为JSON对象（用于文件存储）
    QJsonObject toJson() const;
    // 从JSON对象解析（用于读取文件）
    static Member fromJson(const QJsonObject &json);

private:
    QString m_id;       // 会员ID（唯一标识）
    QString m_name;     // 会员姓名
    QString m_phone;    // 联系电话
    QString m_joinDate; // 加入日期（格式：YYYY-MM-DD）
};

#endif // MEMBER_H
