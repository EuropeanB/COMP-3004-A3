#include "simulationmanager.h"
#include <QDebug>
using namespace std;

// Constructor
SimulationManager::SimulationManager(ECS* ecs) : QObject(nullptr), ecs(ecs), currentTimeStep(0) {
    connect(&simulationTimer, &QTimer::timeout, this, &SimulationManager::updateSimulation);
}

// Destructor
SimulationManager::~SimulationManager() {
    qDebug() << "[Simulation] Destroying SimulationManager";
    simulationTimer.stop();

    for (auto& entry : scheduledActions) {
        entry.second.clear();
    }
    scheduledActions.clear();
}

// Handle simulation starts
void SimulationManager::startSimulation() {
    qDebug() << "[Simulation] Simulation starts";
    emit simulationLog("[Simulation] Simulation started");
    currentTimeStep = 0;
    simulationTimer.start(1000);
}

// Handle simulation paused
void SimulationManager::pauseSimulation() {
    qDebug() << "[Simulation] Pausing simulation";
    emit simulationLog("[Simulation] Simulation paused");
    simulationTimer.stop();
}

// Handle simulation continues
void SimulationManager::continueSimulation() {
    qDebug() << "[Simulation] Resuming simulation";
    emit simulationLog("[Simulation] Simulation resumed");
    simulationTimer.start(1000);
}

// Handle simulation stopped
void SimulationManager::stopSimulation() {
    qDebug() << "[Simulation] Stopping simulation";
    emit simulationLog("[Simulation] Simulation stopped");
    simulationTimer.stop();
    currentTimeStep = 0;

    for (auto& entry : scheduledActions) {
        entry.second.clear();
    }
    scheduledActions.clear();
}

// Called by mainWindow, add passenger action at specific time step
void SimulationManager::addPassengerAction(int timeStep, Passenger* passenger, PassengerAction action) {
    scheduledActions[timeStep].push_back({passenger, action});
}

// Update process
void SimulationManager::updateSimulation() {
    qDebug() << "||------------------------------------||";
    qDebug() << "[Simulation] Time Step " << currentTimeStep;
    emit simulationLog("||------------------------------------||");
    emit simulationLog(QString("[Simulation] Time Step %1").arg(currentTimeStep));

    // Handle passenger movement
    if (scheduledActions.count(currentTimeStep)) {
        for (auto& actionPair : scheduledActions[currentTimeStep]) {
            Passenger* passenger = actionPair.first;
            PassengerAction action = actionPair.second;
            passenger->performAction(ecs, action);
        }
    }

    // Handle elevator moving
    for (int i = 1; i <= ecs->getElevatorCount(); i++) {
        Elevator* e = ecs->getElevator(i);
        if (e && e->getStatus() == ElevatorStatus::Moving) {
            if (firstMove[i]) {
                firstMove[i] = false;
                qDebug() << "[Simulation] Elevator" << i << "first move skipped.";
            } else {
                e->moveStep();
            }
         }
     }

    currentTimeStep++;
}

// Due to the calling moveStep() in updateSimulation(), elevator will suddenly move 1 step when status changed to Moving
map<int, bool> SimulationManager::firstMove;

void SimulationManager::setFirstMove(int elevatorId, bool status) {
    firstMove[elevatorId] = status;
}


