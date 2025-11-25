#ifndef ADDEDITMEMBERDIALOG_H
#define ADDEDITMEMBERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QRegExpValidator>
#include "Member.h"

class AddEditMemberDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { AddMode, EditMode };

    // 构造函数（添加模式）
    AddEditMemberDialog(Mode mode, QWidget *parent = nullptr);
    // 构造函数（编辑模式）
    AddEditMemberDialog(Mode mode, const Member& member, QWidget *parent = nullptr);

    // 获取输入的会员数据
    Member getMember() const;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    void initUI();
    void fillData();
    bool validateInput();

    Mode m_mode;
    Member m_member;

    // UI组件
    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *phoneLabel;
    QLabel *joinDateLabel;

    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *joinDateEdit;

    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
};

#endif // ADDEDITMEMBERDIALOG_H
