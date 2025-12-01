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

    AddEditMemberDialog(Mode mode, QWidget *parent = nullptr);
    AddEditMemberDialog(Mode mode, const Member& member, QWidget *parent = nullptr);

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

    // 新增有效期相关UI
    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *phoneLabel;
    QLabel *joinDateLabel;
    QLabel *expiryDateLabel; // 新增

    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *joinDateEdit;
    QLineEdit *expiryDateEdit; // 新增

    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
};

#endif // ADDEDITMEMBERDIALOG_H
