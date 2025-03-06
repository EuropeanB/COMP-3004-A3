#include "floor.h"
#include "ecs.h"
#include <QDebug>

// Reference: ELevatorAnalysisAndDesign website
// https://brightspace.carleton.ca/d2l/le/content/332132/viewContent/4131069/View

// Constructor
Floor::Floor(int floorNumber, ECS* ecs)
    : floorNumber(floorNumber), ecs(ecs) {
    upButton = new FloorButton(floorNumber, ButtonType::Up, this);
    downButton = new FloorButton(floorNumber, ButtonType::Down, this);
}

// Destructor
Floor::~Floor() {
    delete upButton;
    delete downButton;
}

// Called by floor button, call ECS's floorRequest()
void Floor::inform(ButtonType type) {
    qDebug() << "[Floor] Floor " << floorNumber
             << " button " << (type == ButtonType::Up ? "Up" : "Down") << " pressed";

    if (ecs) {
        char direction = (type == ButtonType::Up) ? 'U' : 'D';
        ecs->floorRequest(floorNumber, direction);
    }
}

// Called by ECS, turn button light off
void Floor::serviced(char direction) {
    if (direction == 'U' && upButton->isLightOn()) {
        upButton->resetButton();
    } else if (direction == 'D' && downButton->isLightOn()) {
        downButton->resetButton();
    }
}

// getters
int Floor::getFloorNumber() const {
    return floorNumber;
}

FloorButton* Floor::getUpButton() {
    return upButton;
}

FloorButton* Floor::getDownButton() {
    return downButton;
}
