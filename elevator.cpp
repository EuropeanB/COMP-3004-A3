#include "elevator.h"
#include "ecs.h"

#include <QDebug>
#include <QTimer>

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
Elevator::Elevator(int id, ECS* ecs)
    : //QObject(nullptr),
    id(id), currentFloor(1), targetFloor(1), status(ElevatorStatus::Idle), ecs(ecs) {
    sensor = new Sensor(this);
    door = new Door(this);
    display = new Display(this);
    carButtonPanel = new CarButtonPanel(this);
    //connect(&moveTimer, &QTimer::timeout, this, &Elevator::moveStep);
}

// Destructor
Elevator::~Elevator() {
    delete sensor;
    delete door;
    delete display;
    delete carButtonPanel;
}

// Called by ECS, let elevator starts to move
void Elevator::moveToFloor(int target) {
    if (target == currentFloor) return;

    targetFloor = target;
    status = ElevatorStatus::Moving;
    qDebug() << "[Elevator] Elevator" << id << "starting movement to floor" << targetFloor;
    SimulationManager::setFirstMove(id, true);
    //qDebug() << "----------------------------------------";

    //moveTimer.start(1000);
}

// Move 1 floor per time step
void Elevator::moveStep() {
    if (currentFloor < targetFloor) {
        currentFloor++;
    } else if (currentFloor > targetFloor) {
        currentFloor--;
    }

    // Send the info to sensor, then sensor call newFloor function
    sensor->detectPosition();

    qDebug() << "[Elevator] Elevator" << id << "moved to floor" << currentFloor;
    //qDebug() << "----------------------------------------";

    /*if (currentFloor == targetFloor) {
        moveTimer.stop();
        status = ElevatorStatus::Arrived;
        qDebug() << "[Elevator] Elevator" << id << "arrived at floor" << currentFloor;
    }*/
}

// Called by sensor
void Elevator::newFloor(int floorNumber) {
    //if (ecs) {
    display->updateTo(floorNumber);
        ecs->newFloor(floorNumber, id);
    //}
}

// Called by ECS, stop this elevator
void Elevator::stop() {
    status = ElevatorStatus::Arrived;
    qDebug() << "[Elevator] Elevator" << id << "arrived at floor" << currentFloor;
    //qDebug() << "----------------------------------------";
    // Unused
    moveTimer.stop();
    status = ElevatorStatus::Arrived;

    //if (door) {
            door->open();
        //}

    //if (ecs) {
            //ecs->readyToMove(id);
        //}
}

// Called by door, tell ECS ready to move
void Elevator::onDoorClosed() {
    status = ElevatorStatus::Idle;
    qDebug() << "[Elevator] Doors closed, ready to move.";
    //qDebug() << "----------------------------------------";
    //if (ecs) {
            ecs->readyToMove(id);
        //}
}

// Handle open door button
void Elevator::openDoor(){
    qDebug() << "[Elevator] passenger pressed open door button";
    //if (door) {
            door->openButton(2000);
        //}
}

// Handle close door button
void Elevator::closeDoor(){
    qDebug() << "[Elevator] passenger pressed close door button";
    //if (door) {
            door->closeButton();
        //}
}

// Handle internal request
void Elevator::carRequest(int floorNumber) {
    qDebug() << "[Elevator] Car request received for floor" << floorNumber;
    ecs->carRequest(floorNumber, id);
}

// Adder
void Elevator::addInternalRequest(int floor){
    internalRequests.insert(floor);
}

// Check
bool Elevator::hasInternalRequest(int floor){
    return internalRequests.find(floor) != internalRequests.end();
}

// Remove one request when arrived
void Elevator::clearInternalRequest(int floor){
    internalRequests.erase(floor);
}

// Check
bool Elevator::internalRequestsEmpty(){
    return internalRequests.empty();
}

// Car button light off
void Elevator::resetCarButton(int floor){
    if(carButtonPanel){
        carButtonPanel->resetButton(floor);
    }
}

// getters
int Elevator::getId() const {
    return id;
}

int Elevator::getCurrentFloor() const {
    return currentFloor;
}

int Elevator::getTargetFloor() const {
    return targetFloor;
}

ElevatorStatus Elevator::getStatus() const {
    return status;
}

CarButtonPanel* Elevator::getCarButtonPanel(){
    return carButtonPanel;
}

int Elevator::getNextInternalRequest(){
return *internalRequests.begin();
}

ECS* Elevator::getECS() {
    return ecs;
}

Display* Elevator::getDisplay(){
    return display;
}

Door* Elevator::getDoor(){
return door;
}

// Handle emergency events
void Elevator::handleEmergency() {
    qDebug() << "[Elevator] Elevator" << id << " entering emergency mode! Moving to safety floor.";

    clearCarRequests();

    moveToFloor(1);
    addInternalRequest(1);

    //display->showWarning("Emergency! Please evacuate at floor 1.");
}

void Elevator::clearCarRequests() {
    qDebug() << "[Elevator] Clearing all internal button requests.";
    internalRequests.clear();
}

// Handle overload event
void Elevator::toggleOverload() {
    if (status != ElevatorStatus::Arrived) return;

    overloaded = !overloaded;
    qDebug() << "[Elevator] Elevator" << id
             << (overloaded ? "Overloaded" : "Overload removed.");

    if (overloaded) {
        QString logMsg = QString("[Elevator] Elevator %1 Overload, Reduce weight to continue").arg(id);
        qDebug() << logMsg;
        display->showWarning(logMsg);
        door->open();
    } else {
        QString logMsg = QString("[Elevator] Elevator %1 is no longer overloaded").arg(id);
        qDebug() << logMsg;
        display->showWarning(logMsg);
        door->close();
    }
}

bool Elevator::isOverloaded() const {
    return overloaded;
}








