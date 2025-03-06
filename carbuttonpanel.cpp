#include "carbuttonpanel.h"
#include "elevator.h"

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
CarButtonPanel::CarButtonPanel(Elevator* parentElevator) : elevator(parentElevator) {}

// Button: press
void CarButtonPanel::pressButton(int floor) {
    if (buttonStates[floor]) return;
    buttonStates[floor] = true;

    qDebug() << "[CarButtonPanel] Car button for floor" << floor << " pressed and light on.";
    elevator->carRequest(floor);
}

// Button: light off
void CarButtonPanel::resetButton(int floor) {
    if (!buttonStates[floor]) return;
    buttonStates[floor] = false;

    qDebug() << "[CarButtonPanel] Car button for floor" << floor << " reset and light off.";
}

// Check button light
bool CarButtonPanel::isButtonLightOn(int floor) const {
    // auto: map<int, bool>
    auto it = buttonStates.find(floor);
    return (it != buttonStates.end()) && it->second;
}









