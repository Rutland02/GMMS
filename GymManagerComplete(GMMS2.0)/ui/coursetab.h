#ifndef COURSETAB_H
#define COURSETAB_H

#include <QWidget>
class GymData;
class QTableWidget;

class CourseTab : public QWidget {
    Q_OBJECT
public:
    explicit CourseTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private:
    GymData *data;
    QTableWidget *table;
};

#endif // COURSETAB_H
