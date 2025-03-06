#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include <map>
#include <QObject>
#include <QTimer>
#include <vector>

#include "ecs.h"
#include "passenger.h"

using namespace std;

class SimulationManager : public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    SimulationManager(ECS* ecs);
    ~SimulationManager();

    // Handle simulation control
    void startSimulation();
    void pauseSimulation();
    void continueSimulation();
    void stopSimulation();

    // Handle passenger
    void addPassengerAction(int timeStep, Passenger* passenger, PassengerAction action);
    static void setFirstMove(int elevatorId, bool status);

signals:
    // To logCOnsole
    void simulationLog(const QString& message);

private slots:
    // Handle passenger and elevator moving that affected by time step
    void updateSimulation();

private:
    ECS* ecs;
    QTimer simulationTimer;
    int currentTimeStep;
    // Layout: int: time step, vector<pair: store all operations corresponding to this time step
    // Example: {1, [(id: 1, FloorRequest: 3, 'U'),(id: 2, FloorRequest: 1, 'U')]}, {2...}
    map<int, vector<pair<Passenger*, PassengerAction>>> scheduledActions;
    static map<int, bool> firstMove;
};

#endif // SIMULATIONMANAGER_H

