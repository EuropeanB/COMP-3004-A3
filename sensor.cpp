#include "sensor.h"
#include "elevator.h"
#include <QDebug>

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
Sensor::Sensor(Elevator* elevator) : elevator(elevator) {}

// Called by elevator, call elevator's newFloor()
void Sensor::detectPosition() {
    int detectedFloor = elevator->getCurrentFloor();
    qDebug() << "[Sensor] Elevator" << elevator->getId() << "detected at floor" << detectedFloor;

    elevator->newFloor(detectedFloor);
}
