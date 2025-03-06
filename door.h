#ifndef DOOR_H
#define DOOR_H

#include <QTimer>
#include <QDebug>
#include <QObject>

class ECS;
class Elevator;

class Door : public QObject {
    Q_OBJECT

public:
    // Constructor
    Door(Elevator* elevator);

    // Door open/close
    void open();
    void close();

    // Open/close by admin
    void openButton(int extraTime);
    void closeButton();

    // Handle obstacle event
    void addRemoveObstacle();
    bool hasObstacle() const;

signals:
    // To logConsole
    void doorLog(const QString& message);

private:
    Elevator* elevator;

    bool obstacle = false;
    int obstacleCount = 0;

    QTimer* doorTimer;
};

#endif // DOOR_H
