#include "passengeraction.h"

// Constructor
PassengerAction::PassengerAction(PassengerActionType type, int floorNumber, char direction)
    : type(type), floorNumber(floorNumber), direction(direction) {
    // Unused
    if (type == PassengerActionType::CarRequest ||
        type == PassengerActionType::OpenDoor ||
        type == PassengerActionType::CloseDoor) {
        this->direction = '\0';
    }
}

// getters
PassengerActionType PassengerAction::getType() const {
    return type;
}

int PassengerAction::getFloorNumber() const {
    return floorNumber;
}

char PassengerAction::getDirection() const {
    return direction;
}
