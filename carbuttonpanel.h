#ifndef CARBUTTONPANEL_H
#define CARBUTTONPANEL_H

#include <map>
#include <QDebug>
using namespace std;

class Elevator;

class CarButtonPanel {
public:
    // Constructor
    CarButtonPanel(Elevator* parentElevator);

    // Handle buttons
    void pressButton(int floor);
    void resetButton(int floor);

    // Check button light
    bool isButtonLightOn(int floor) const;

private:
    Elevator* elevator;

    map<int, bool> buttonStates;
};

#endif // CARBUTTONPANEL_H
