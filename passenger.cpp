#include "passenger.h"
#include "ecs.h"
#include <QDebug>

// Constructor, destructor is in ECS
Passenger::Passenger(int id, int startFloor, int destinationFloor)
    : id(id), startFloor(startFloor), destinationFloor(destinationFloor), currentElevatorId(-1) {}

// Called by simulationManager
void Passenger::performAction(ECS* ecs, PassengerAction action) {
    QString logMsg;
    // Action type
    switch (action.getType()) {
        case PassengerActionType::FloorRequest: {
            int floorNumber = action.getFloorNumber();
            char direction = action.getDirection();
            Floor* floor = ecs->getFloor(floorNumber);

            logMsg = QString("[Passenger] %1 presses floor button at %2").arg(id).arg(action.getFloorNumber());

                // Press floor button
                if (direction == 'U') {
                    floor->getUpButton()->pressButton();
                } else {
                    floor->getDownButton()->pressButton();
                }
            break;
        }

        // Unused
        case PassengerActionType::CarRequest:
            logMsg = QString("[Passenger] %1 presses car button for floor %2").arg(id).arg(action.getFloorNumber());
            if (currentElevatorId != -1) {
                ecs->carRequest(action.getFloorNumber(), currentElevatorId);
            }
            break;

        // Unused
        case PassengerActionType::OpenDoor:
            logMsg = QString("[Passenger] %1 presses Open Door button").arg(id);
            if (currentElevatorId != -1) {
                ecs->getElevator(currentElevatorId)->openDoor();
            }
            break;

        // Unused
        case PassengerActionType::CloseDoor:
            logMsg = QString("[Passenger] %1 presses Close Door button").arg(id);
            if (currentElevatorId != -1) {
                ecs->getElevator(currentElevatorId)->closeDoor();
            }
            break;
    }

    qDebug() << logMsg;
    emit ecs->ECSLog(logMsg);
}

// Handle passenger boards elevator
void Passenger::enterElevator(int elevatorId, ECS* ecs) {
    currentElevatorId = elevatorId;
    QString logMsg = QString("[Passenger] Passenger %1 boards elevator %2").arg(id).arg(elevatorId);

    qDebug() << logMsg;
    emit ecs->ECSLog(logMsg);
}

// Hanndle passenger exits elevator
void Passenger::exitElevator(ECS* ecs) {
    if (currentElevatorId == -1) {
        QString logMsg = QString("[Passenger] Passenger %1 is not in an elevator").arg(id);
        qDebug() << logMsg;
        emit ecs->ECSLog(logMsg);
        return;
    }

    QString logMsg = QString("[Passenger] Passenger %1 exits elevator %2").arg(id).arg(currentElevatorId);

    qDebug() << logMsg;
    emit ecs->ECSLog(logMsg);
    currentElevatorId = -1;
    // Delete passenger
    ecs->removePassenger(id);
}

// Check
bool Passenger::isInElevator() const {
    return currentElevatorId != -1;
}

// getters
int Passenger::getDestinationFloor() const {
    return destinationFloor;
}

int Passenger::getCurrentFloor() const {
    if (currentElevatorId == -1) return startFloor;
    return -1;
}

int Passenger::getId() const{
    return id;
}

int Passenger::getElevatorId() const {
    return currentElevatorId;
}
