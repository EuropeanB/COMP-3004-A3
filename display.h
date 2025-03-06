#ifndef DISPLAY_H
#define DISPLAY_H

#include <QDebug>
#include <QObject>

class Elevator;

class Display: public QObject {
    Q_OBJECT
public:
    // Constructor
    Display(Elevator* elevator);

    // Display
    void updateTo(int floorNumber);
    void showWarning(const QString& message);

signals:
    // To logConsole
    void displayLog(const QString& message);

private:
    Elevator* elevator;
};

#endif // DISPLAY_H
