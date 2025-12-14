#ifndef QUERYTAB_H
#define QUERYTAB_H

#include <QWidget>
class GymData;
class QTableWidget;
class QComboBox;
class QDateTimeEdit;
class QLineEdit;

#include <QGroupBox>

class QueryTab : public QWidget {
    Q_OBJECT
public:
    explicit QueryTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private slots:
    void applyFilters();
    void exportData();

private:
    GymData *data;
    QTableWidget *recordTable;
    QComboBox *typeCombo;
    QDateTimeEdit *startDateEdit;
    QDateTimeEdit *endDateEdit;
    QLineEdit *searchEdit;
};

#endif // QUERYTAB_H
