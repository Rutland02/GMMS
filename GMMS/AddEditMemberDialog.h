#ifndef ADDEDITMEMBERDIALOG_H
#define ADDEDITMEMBERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Member.h"

class AddEditMemberDialog : public QDialog
{
    Q_OBJECT

public:
    // 模式：添加（0）/ 编辑（1）
    enum Mode { AddMode = 0, EditMode = 1 };

    // 构造函数：添加模式（无需初始会员）
    explicit AddEditMemberDialog(Mode mode, QWidget *parent = nullptr);
    // 构造函数：编辑模式（需要传入初始会员数据）
    AddEditMemberDialog(Mode mode, const Member &member, QWidget *parent = nullptr);

    // 获取用户输入的会员数据
    Member getMemberData() const;

private slots:
    // 确认按钮事件
    void onConfirmClicked();
    // 取消按钮事件
    void onCancelClicked();

private:
    Mode m_mode;              // 对话框模式
    Member m_member;          // 存储会员数据

    // UI组件
    QLabel *idLabel;
    QLineEdit *idEdit;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *phoneLabel;
    QLineEdit *phoneEdit;
    QLabel *joinDateLabel;
    QLineEdit *joinDateEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    // 初始化UI
    void initUI();
    // 填充编辑模式的初始数据
    void fillMemberData(const Member &member);
    // 校验输入数据是否合法
    bool validateInput();
};

#endif // ADDEDITMEMBERDIALOG_H
