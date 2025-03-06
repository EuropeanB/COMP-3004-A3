#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <set>
#include <QTimer>
#include <QObject>

#include "door.h"
#include "sensor.h"
#include "display.h"
#include "carbuttonpanel.h"
#include "simulationmanager.h"

enum class ElevatorStatus { Idle, Moving, Arrived };

class ECS;

class Elevator//: public QObject
{
    //Q_OBJECT

public:
    // Constructor and destructor
    Elevator(int id, ECS* ecs);
    ~Elevator();

    // Handle moving
    void moveToFloor(int target);
    void moveStep();
    void newFloor(int floorNumber);
    void stop();
    void onDoorClosed();

    // Handle door open/close
    void openDoor();
    void closeDoor();

    // Handle internal requests
    void carRequest(int floorNumber);
    void addInternalRequest(int floor);
    bool hasInternalRequest(int floor);
    void clearInternalRequest(int floor);
    bool internalRequestsEmpty();
    int getNextInternalRequest();

    // Handle car button panel
    void resetCarButton(int floor);

    // getters
    int getId() const;
    int getCurrentFloor() const;
    int getTargetFloor() const;

    // getters
    ElevatorStatus getStatus() const;
    CarButtonPanel* getCarButtonPanel();
    ECS* getECS();
    Display* getDisplay();
    Door* getDoor();

    // Handle emergency event
    void handleEmergency();
    void clearCarRequests();

    // Handle overload event
    void toggleOverload();
    bool isOverloaded() const;

//public slots:
    //void moveStep();

private:
    int id;
    int currentFloor;
    int targetFloor;
    bool overloaded = false;
    set<int> internalRequests;

    ElevatorStatus status;
    Sensor* sensor;
    ECS* ecs;
    QTimer moveTimer;
    Door* door;
    Display* display;
    CarButtonPanel* carButtonPanel;
};

#endif // ELEVATOR_H
