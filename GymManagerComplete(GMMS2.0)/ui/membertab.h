#ifndef MEMBERTAB_H
#define MEMBERTAB_H

#include <QWidget>
class GymData;
class QTableWidget;

class MemberTab : public QWidget {
    Q_OBJECT
public:
    explicit MemberTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private:
    GymData *data;
    QTableWidget *table;
};

#endif // MEMBERTAB_H
