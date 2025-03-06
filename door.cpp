#include "door.h"
#include "elevator.h"
#include "ecs.h"

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
Door::Door(Elevator* elevator) : QObject(nullptr), elevator(elevator) {
    doorTimer = new QTimer(this);
    doorTimer->setSingleShot(true);
    connect(doorTimer, &QTimer::timeout, this, &Door::close);
}

// Door open
void Door::open() {
    //qDebug() << "[Door] Opening...";
    qDebug() << "[Bell] Ding! (Door Opening)";

    QString logMsg = QString("[Door] Elevator %1 Door open").arg(elevator->getId());
    qDebug() << logMsg;
    emit doorLog(logMsg);

    ECS* ecs = elevator->getECS();
    vector<Passenger*> passengers = elevator->getECS()->getPassengers();

    int currentFloor = elevator->getCurrentFloor();
    bool emergencyActive = elevator->getECS()->isEmergencyActive();

    // Handle passenger exits
    for (auto it = passengers.begin(); it != passengers.end();) {
        Passenger* p = *it;

        // Emergency event
        if (emergencyActive && currentFloor == 1) {
            p->exitElevator(ecs);
            qDebug() << "[Door Emergency] Passenger " << p->getId() << " is forced to exit at floor " << currentFloor;
            it = passengers.erase(it);
            continue;
        }

        // Normal exit
        if (p->isInElevator() && p->getDestinationFloor() == currentFloor) {
            int pid = p->getId();
            p->exitElevator(ecs);
            qDebug() << "[Door] Passenger" << pid << " exits elevator at floor " << currentFloor;
            it = passengers.erase(it);
            continue;
        }

        ++it;
    }

    //qDebug() << "[Door Debug] Number of passengers: " << passengers.size();

    // Handle passenger boards
    for (Passenger* p : passengers) {
        if (p->getCurrentFloor() == currentFloor && !p->isInElevator()) {
            p->enterElevator(elevator->getId(),ecs);
            qDebug() << "[door] Passenger" << p->getId() << " boards elevator" << elevator->getId();

            int targetFloor = p->getDestinationFloor();
            qDebug() << "[door] Passenger" << p->getId() << " presses car button for floor" << targetFloor;
            elevator->getCarButtonPanel()->pressButton(targetFloor);
        }
    }
    // Open for three steps
    doorTimer->start(3000);
}

// Door close
void Door::close() {
    // Overload event
    if (elevator->isOverloaded()) {
            qDebug() << "[Door] Cannot close, elevator is overloaded";
            return;
        }

    // Obstacle event
    if (obstacle) {
        obstacleCount++;
        QString logMsg = QString("[Door] Elevator %1 detected obstacle").arg(elevator->getId());
        qDebug() << logMsg;
        open();

        // more than 3 times
        if (obstacleCount >= 3) {
            //qDebug() << "[Door] Obstacle detected multiple times!";
            QString logMsg = QString("[Door] Elevator %1 detected obstacle multiple times").arg(elevator->getId());
            qDebug() << logMsg;
            elevator->getDisplay()->showWarning(logMsg);
         }
         return;
    }

    //qDebug() << "[Door] Closing...";
    qDebug() << "[Bell] Ding! (Door Closing)";

    QString logMsg = QString("[Door] Elevator %1 Door close").arg(elevator->getId());
    qDebug() << logMsg;
    emit doorLog(logMsg);

    obstacleCount = 0;

    //if (elevator) {
        elevator->onDoorClosed();
    //}
}

// Open by admin
void Door::openButton(int extraTime) {
    if (doorTimer->isActive()) {
        doorTimer->start(doorTimer->remainingTime() + extraTime);
    }
    qDebug() << "[Door] Extending open time by" << extraTime << "ms";
}

// Close by admin
void Door::closeButton() {
    if (doorTimer->isActive()) {
        doorTimer->stop();
    }
    close();
}

// Handle Obstacle
void Door::addRemoveObstacle() {
    if (elevator->getStatus() != ElevatorStatus::Arrived) return;
    obstacle = !obstacle;
}

// Handle Obstacle
bool Door::hasObstacle() const {
    return obstacle;
}
