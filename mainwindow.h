#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

#include "ecs.h"
#include "door.h"
#include "floor.h"
#include "display.h"
#include "elevator.h"
#include "passenger.h"
#include "logconsole.h"
#include "simulationmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Adders
    void addElevator();
    void addFloor();
    void addPassenger();

    // Updaters
    void updateUI();
    void updateElevatorStatus();

    // setters
    void setupFloorButtons();
    void setupCarButtons();

    // Simulation controller
    void startSimulation();
    void pauseSimulation();
    void continueSimulation();
    void stopSimulation();

    // Console log
    void logToConsole(const QString& message);

    // Handle emergency events
    void triggerFireAlert();
    void triggerPowerOut();

private:
    Ui::MainWindow *ui;
    ECS *ecs;
    LogConsole* logConsole;
    SimulationManager* simulationManager;
    //Display* display;
    int floorCount = 1;
    int passengerCount = 0;

};

#endif // MAINWINDOW_H

