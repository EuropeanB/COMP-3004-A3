#include "display.h"
#include "elevator.h"

// Constructor
Display::Display(Elevator* elevator) :QObject(nullptr),  elevator(elevator) {}

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Display
void Display::updateTo(int floorNumber) {
    qDebug() << "[Display] Elevator" << elevator->getId() << "is now at floor" << floorNumber;
}

// Display
void Display::showWarning(const QString& message) {
    //qDebug() << "[Display Warning] " << message;
    //emit displayLog(message);
    QString logMsg = QString("[Display Warning] %1").arg(message);
    qDebug() << logMsg;

    // To logConsole
    emit displayLog(logMsg);
}



