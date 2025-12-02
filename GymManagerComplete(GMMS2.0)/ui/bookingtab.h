#ifndef BOOKINGTAB_H
#define BOOKINGTAB_H

#include <QWidget>
class GymData;
class QComboBox;
class QTableWidget;

class BookingTab : public QWidget {
    Q_OBJECT
public:
    explicit BookingTab(GymData *data, QWidget *parent = nullptr);

public slots:
    void refresh();

private:
    GymData *data;
    QComboBox *memberCombo;
    QComboBox *courseCombo;
    QTableWidget *recordTable;

    void handleBooking(bool isBooking);
};

#endif // BOOKINGTAB_H
