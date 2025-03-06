#ifndef FLOORBUTTON_H
#define FLOORBUTTON_H

#include <iostream>

enum class ButtonType { Up, Down, Internal};

class Floor;

class FloorButton {
public:
    // Constructor
    FloorButton(int floor, ButtonType type, Floor* parentFloor);

    // Handle buttons
    void pressButton();
    void resetButton();

    // Check button light
    bool isLightOn() const;

    //getters
    int getFloor() const;
    ButtonType getType() const;

private:
    int floor;
    ButtonType type;
    bool pressed;
    bool lightOn;
    Floor* floorRef;
};

#endif // FLOORBUTTON_H
