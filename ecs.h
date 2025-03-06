#ifndef ECS_H
#define ECS_H

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include "floor.h"
#include "elevator.h"
#include "passenger.h"
#include "logconsole.h"

using namespace std;

enum class EmergencyType {
    fireAlarm, powerOut
};

class ECS: public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    ECS(int initialElevators, int initialFloors);
    ~ECS();

    // Handle elevator requrest and moving
    void floorRequest(int floorNumber, char direction);
    void carRequest(int floorNumber, int carNumber);
    void newFloor(int floorNumber, int elevatorId);
    void makeDecision(int elevatorId, int floorNumber);
    void readyToMove(int elevatorId);

    // Adders
    void addElevator();
    void addFloor();
    void addPassenger(Passenger* passenger);
    void removePassenger(int passengerId);

    // getters
    int getElevatorCount() const;
    int getFloorCount() const;
    string getElevatorStatus();
    Floor* getFloor(int floorNumber);
    Elevator* getElevator(int id);
    vector<Passenger*> getPassengers();

    // Handle emergency events
    void triggerEmergency(EmergencyType type);
    bool isEmergencyActive();
    void clearFloorRequests();

signals:
    void ECSLog(const QString& message);

private:
    bool emergencyActive = false;

    vector<Passenger*> passengers;
    vector<Elevator*> elevators;
    vector<Floor*> floors;
    // Layout: {3, {'U','D'}}, {4,{'U'}}
    map<int, set<char>> requestedFloors;
};

#endif // ECS_H

