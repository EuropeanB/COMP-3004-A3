#include "ecs.h"
//#include "logconsole.h"

#include <iostream>
#include <QDebug>
#include <limits>

using namespace std;

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
ECS::ECS(int initialElevators, int initialFloors): QObject(nullptr) {
    for (int i = 0; i < initialElevators; ++i) {
        elevators.push_back(new Elevator(i + 1, this));
    }
    for (int i = 0; i < initialFloors; ++i) {
        floors.push_back(new Floor(i + 1, this));
    }
}

// Destructor
ECS::~ECS() {
    for (Elevator* e : elevators) {
        delete e;
    }
    for (Floor* f : floors) {
        delete f;
    }
}

// Receive requirement from floor
void ECS::floorRequest(int floorNumber, char direction) {
    //qDebug() << "[ECS] Floor " << floorNumber << " request, direction: " << direction;
    QString logMsg = QString("[ECS] Floor %1 request, direction: %2").arg(floorNumber).arg(direction);
    qDebug() << logMsg;
    emit ECSLog(logMsg);

    // At same floor
    for (Elevator* e : elevators) {
        if (e->getCurrentFloor() == floorNumber) {
            //qDebug() << "[ECS] Elevator" << e->getId() << "is already at floor" << floorNumber;
            QString logMsg = QString("[ECS] elevator %1 is already at floor").arg(e->getId());
            qDebug() << logMsg;
            emit ECSLog(logMsg);

            Floor* floor = getFloor(floorNumber);
            //if (floor) {
                floor->serviced(direction);
            //}

            e->stop();

            /* Unsed edge condition
            if (requestedFloors.find(floorNumber) != requestedFloors.end()) {
                requestedFloors[floorNumber].erase(direction);
                if (requestedFloors[floorNumber].empty()) {
                    requestedFloors.erase(floorNumber);
                }
            }*/
            return;
        }
    }

    // Store request
    requestedFloors[floorNumber].insert(direction);

    // Call the clostest elevator
    Elevator* bestElevator = nullptr;
    int minDistance = INT_MAX;

    for (Elevator* e : elevators) {
        if (e->getStatus() == ElevatorStatus::Idle ||
            // Same direction
            (e->getStatus() == ElevatorStatus::Moving &&
            ((direction == 'U' && e->getTargetFloor() > e->getCurrentFloor()) ||
            (direction == 'D' && e->getTargetFloor() < e->getCurrentFloor())))) {

            // abs: absolute value
            int distance = abs(e->getCurrentFloor() - floorNumber);
            if (distance < minDistance) {
                minDistance = distance;
                bestElevator = e;
            }
        }
    }

    // Found/Not found
    if (bestElevator && bestElevator->getStatus() == ElevatorStatus::Idle) {
        qDebug() << "[ECS] Assigning Elevator" << bestElevator->getId() << "to floor" << floorNumber;
        bestElevator->moveToFloor(floorNumber);
    } else {
        qDebug() << "[ECS] No available idle elevator, storing request for later";
        //qDebug() << "----------------------------------------";
    }
}

// Received request from elevator
void ECS::carRequest(int floorNumber, int carNumber) {
    //qDebug() << "[ECS] Car request: Elevator" << carNumber << " requested to floor " << floorNumber;
    QString logMsg = QString("[ECS] Car request Elevator %1 requested to floor %2").arg(carNumber).arg(floorNumber);
    qDebug() << logMsg;
    emit ECSLog(logMsg);

    // Find the correct one
    for (Elevator* e : elevators) {
        if (e->getId() == carNumber) {
            // At same floor
            if (e->getCurrentFloor() == floorNumber) {
                //qDebug() << "[ECS] Elevator" << carNumber << " is already at floor " << floorNumber;
                QString logMsg = QString("[ECS] Elevator %1 is already at floor %2").arg(carNumber).arg(floorNumber);
                qDebug() << logMsg;
                emit ECSLog(logMsg);

                e->resetCarButton(floorNumber);
                e->stop();
            } else {
                // Elevator moving status trrigged by readyToMove()
                qDebug() << "[ECS] Assigning Elevator" << carNumber << " to requested floor " << floorNumber;
                e->addInternalRequest(floorNumber);
            }
            return;
        }
    }
}

// Receive elevator information from elevator and call makeDecision()
void ECS::newFloor(int floorNumber, int elevatorId) {
    //qDebug() << "[ECS] Elevator" << elevatorId << "moved to floor" << floorNumber;
    QString logMsg = QString("[ECS] Elevator %1 moved to floor %2").arg(elevatorId).arg(floorNumber);
    qDebug() << logMsg;
    emit ECSLog(logMsg);
    makeDecision(elevatorId, floorNumber);
}

// Decide whether the elevator should stop. And delete the request
void ECS::makeDecision(int elevatorId, int floorNumber) {
    // Find the correct one
    for (Elevator* e : elevators) {
        if (e->getId() == elevatorId) {
            qDebug() << "[ECS] Checking if Elevator " << elevatorId << " should stop at floor " << floorNumber;
            //QString logMsg = QString("[ECS] Checking if Elevator %1 should stop at floor %2").arg(elevatorId).arg(floorNumber);
            //qDebug() << logMsg;
            //LogConsole::log(logMsg);

            // auto: map<int, set<char>>
            auto it = requestedFloors.find(floorNumber);
            bool shouldStop = false;
            // For internal request
            char requestDirection = '\0';

            // Check floor request
            if (it != requestedFloors.end()) {
                set<char>& directions = it->second;
                requestDirection = (directions.count('U') ? 'U' : 'D');

                // For debug
                bool goingUp = (e->getTargetFloor() > e->getCurrentFloor());
                bool goingDown = (e->getTargetFloor() < e->getCurrentFloor());

                if (!goingUp && !goingDown) {
                    goingUp = (requestDirection == 'U');
                    goingDown = (requestDirection == 'D');
                }

                qDebug() << "[ECS] Elevator " << elevatorId << " status:";
                qDebug() << " - Current floor: " << e->getCurrentFloor();
                qDebug() << " - Target floor: " << e->getTargetFloor();
                qDebug() << " - Going up: " << goingUp;
                qDebug() << " - Going down: " << goingDown;
                qDebug() << " - Request at this floor: " << (directions.count('U') ? "UP " : "")
                                                         << (directions.count('D') ? "DOWN" : "");
                // Should stop and delete the request
                if ((goingUp && directions.count('U')) || (goingDown && directions.count('D'))) {
                    shouldStop = true;

                    if (goingUp) {
                        directions.erase('U');
                    } else if (goingDown) {
                        directions.erase('D');
                    }

                    if (directions.empty()) {
                        requestedFloors.erase(it);
                    }
                }
            }

            // Check internal request
            if (e->hasInternalRequest(floorNumber)) {
                shouldStop = true;
                e->clearInternalRequest(floorNumber);
            }

            // Handle stop event
            if (shouldStop) {
                //qDebug() << "[ECS] Elevator " << elevatorId << " stopping at floor " << floorNumber;
                QString logMsg = QString("[ECS] Elevator %1 stopping at floor %2").arg(elevatorId).arg(floorNumber);
                qDebug() << logMsg;
                emit ECSLog(logMsg);

                // turn off the floor/elevator button light
                Floor* floor = getFloor(floorNumber);
                if (floor && requestDirection != '\0') {
                    floor->serviced(requestDirection);
                }

                e->resetCarButton(floorNumber);

                e->stop();

            } else {
                // Keeping moving
                qDebug() << "[ECS] Elevator " << elevatorId << " will NOT stop at floor " << floorNumber;
            }
            return;
        }
    }
}

// Called by elevator, it's ready to move
void ECS::readyToMove(int elevatorId) {
    // find the correct one
    for (Elevator* e : elevators) {
        if (e->getId() == elevatorId) {
            qDebug() << "[ECS] readyToMove: Checking next target for Elevator" << elevatorId;

            // Handle internal request first
            if (!e->internalRequestsEmpty()) {
                int nextFloor = e->getNextInternalRequest();
                qDebug() << "[ECS] Elevator " << elevatorId << " proceeding to internal request floor " << nextFloor;
                e->moveToFloor(nextFloor);
                return;
            }

            // Handle floor request
            if (!requestedFloors.empty()) {
                int nextFloor = 0;
                int minDistance = INT_MAX;

                // Find the closest request
                // map<int, set<char>>
                for (auto& req : requestedFloors) {
                    int floor = req.first;
                    // abs: absolute value
                    int distance = abs(floor - e->getCurrentFloor());

                    if (distance < minDistance) {
                        minDistance = distance;
                        nextFloor = floor;
                    }
                }

                // Request found
                if (nextFloor != 0) {
                    //qDebug() << "[ECS] Assigning Elevator" << elevatorId << "to next request at floor" << nextFloor;
                    QString logMsg = QString("[ECS] Assigning Elevator %1 to next request at floor %2").arg(elevatorId).arg(nextFloor);
                    qDebug() << logMsg;
                    emit ECSLog(logMsg);
                    e->moveToFloor(nextFloor);
                    return;
                }
            }

            // Requet not found
            qDebug() << "[ECS] Elevator" << elevatorId << " is idle and has no pending requests";
            //qDebug() << "----------------------------------------";
            return;
        }
    }
}

//Adders
void ECS::addElevator() {
    elevators.push_back(new Elevator(elevators.size() + 1, this));
}

void ECS::addFloor() {
    floors.push_back(new Floor(floors.size() + 1, this));
}

void ECS::addPassenger(Passenger* passenger) {
    passengers.push_back(passenger);
    qDebug() << "[ECS] Added passenger " << passenger->getId();
}

// Delete the passenger (passenger's destructor)
void ECS::removePassenger(int passengerId) {

    for (auto it = passengers.begin(); it != passengers.end();) {
        if ((*it)->getId() == passengerId) {
            delete *it;
            it = passengers.erase(it);
            qDebug() << "[ECS] Passenger" << passengerId << " completely removed.";
        } else {
            ++it;
        }
    }
}

// getters
// For UI display
string ECS::getElevatorStatus() {
    string status;
    for (Elevator* e : elevators) {
        status += "Elevator " + to_string(e->getId()) +
                  ": Floor " + to_string(e->getCurrentFloor()) +
                  " - Status: " + (e->getStatus() == ElevatorStatus::Idle ? "Idle" :
                                   e->getStatus() == ElevatorStatus::Moving ? "Moving" : "Arrived") + "\n";
    }
    return status;
}

Floor* ECS::getFloor(int floorNumber) {
    if (floorNumber > 0 && static_cast<size_t>(floorNumber) <= floors.size()) {
        return floors[floorNumber - 1];
    }
    return nullptr;
}

Elevator* ECS::getElevator(int id) {
    if (id > 0 && id <= static_cast<int>(elevators.size())) {
        return elevators[id - 1];
    }
    return nullptr;
}

vector <Passenger*> ECS::getPassengers() {
    qDebug() << "[ECS] Returning " << passengers.size() << " passengers";
    return passengers;
}

int ECS::getFloorCount() const {
    return floors.size();
}

int ECS::getElevatorCount() const {
    return elevators.size();
}

// Handle emergency events
void ECS::triggerEmergency(EmergencyType type) {
    QString message;
    if (type == EmergencyType::fireAlarm) {
        message = "[Emergency] Fire detected! All elevators moving to safety floor.";
    } else {
        message = "[Emergency] Power failure! All elevators moving to safety floor.";
    }

    clearFloorRequests();

    emergencyActive = true;
    emit ECSLog(message);

    for (Elevator* elevator : elevators) {
        elevator->handleEmergency();
    }
}

// Clear all floor requests
void ECS::clearFloorRequests() {
    qDebug() << "[ECS] Clearing all floor requests due to emergency!";
    requestedFloors.clear();
}

// Check the ECS status
bool ECS::isEmergencyActive() {
    return emergencyActive;
}







