#ifndef PASSENGER_H
#define PASSENGER_H

#include "passengeraction.h"


class ECS;

class Passenger {
public:
    // Constructor
    Passenger(int id, int startFloor, int destinationFloor);

    // Define passenger action
    void performAction(ECS* ecs, PassengerAction action);

    // Handle board/exit elevator
    void enterElevator(int elevatorId, ECS* ecs);
    void exitElevator(ECS* ecs);
    bool isInElevator() const;

    // getters
    int getDestinationFloor() const;
    int getCurrentFloor() const;
    int getElevatorId() const;
    int getId() const;

private:
    int id;
    int startFloor;
    int destinationFloor;
    int currentElevatorId;
};

#endif // PASSENGER_H

