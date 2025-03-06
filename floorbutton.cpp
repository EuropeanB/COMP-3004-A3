#include "floorbutton.h"
#include "floor.h"
#include <QDebug>
using namespace std;

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
FloorButton::FloorButton(int floor, ButtonType type, Floor* parentFloor)
    : floor(floor), type(type), pressed(false), lightOn(false), floorRef(parentFloor) {}

// Called by passenger, call floor's inform()
void FloorButton::pressButton() {
    if (!pressed) {
        pressed = true;
        lightOn = true;
        qDebug() << "[FloorButton] Floor " << floor
                 << " " << (type == ButtonType::Up ? "Up" : "Down") << " light on";

        //if (floorRef) {
            floorRef->inform(type);
        //}
    }
}

// Called by floor, turn off the button light
void FloorButton::resetButton() {
    pressed = false;
    lightOn = false;
    qDebug() << "[FloorButton] Floor " << floor
             << " " << (type == ButtonType::Up ? "Up" : "Down") << " light off";
}

// Check
bool FloorButton::isLightOn() const {
    return lightOn;
}

// getters
int FloorButton::getFloor() const {
    return floor;
}

ButtonType FloorButton::getType() const {
    return type;
}
