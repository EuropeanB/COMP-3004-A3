#ifndef FLOOR_H
#define FLOOR_H

#include "floorbutton.h"

class ECS;

class Floor {
public:
    // Constructor and destructor
    Floor(int floorNumber, ECS* ecs);
    ~Floor();

    // Handle buttons
    void inform(ButtonType type);
    void serviced(char direction);

    // getters
    int getFloorNumber() const;
    FloorButton* getUpButton();
    FloorButton* getDownButton();

private:
    int floorNumber;
    ECS* ecs;
    FloorButton* upButton;
    FloorButton* downButton;
};

#endif // FLOOR_H
