#ifndef PASSENGERACTION_H
#define PASSENGERACTION_H

#include <QString>

enum class PassengerActionType {
    FloorRequest,
    CarRequest,
    OpenDoor,
    CloseDoor
};

class PassengerAction {
public:
    // Constructor
    PassengerAction(PassengerActionType type, int floorNumber, char direction = '\0');

    // getters
    PassengerActionType getType() const;
    int getFloorNumber() const;
    char getDirection() const;

private:
    PassengerActionType type;
    int floorNumber;
    char direction;
};

#endif // PASSENGERACTION_H
