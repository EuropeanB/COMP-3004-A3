#include "Floor.h"

Floor::Floor(int floorNumber, QObject *parent) : QObject(parent), floorNumber(floorNumber), upRequest(false), downRequest(false) {}

void Floor::pressButton(bool up) {
    if (up) upRequest = true;
    else downRequest = true;
}
