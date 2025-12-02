#ifndef QUERYTAB_H
#define QUERYTAB_H

#include <QWidget>
class GymData;
class QTableWidget;

class QueryTab : public QWidget {
    Q_OBJECT
public:
    explicit QueryTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private:
    GymData *data;
    QTableWidget *recordTable;
};

#endif // QUERYTAB_H
