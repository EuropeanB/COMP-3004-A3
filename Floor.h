#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>

class Floor : public QObject {
    Q_OBJECT

public:
    explicit Floor(int floorNumber, QObject *parent = nullptr);
    void pressButton(bool up);

private:
    int floorNumber;
    bool upRequest;
    bool downRequest;
};

#endif // FLOOR_H
