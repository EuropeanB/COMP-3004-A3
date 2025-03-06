#ifndef SENSOR_H
#define SENSOR_H

class Elevator;

class Sensor {
public:
    // Constructor
    Sensor(Elevator* elevator);

    // Detect
    void detectPosition();

private:
    Elevator* elevator;
};

#endif // SENSOR_H
