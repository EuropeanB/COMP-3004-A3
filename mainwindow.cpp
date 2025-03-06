#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>

// References: Qt Group
// https://wiki.qt.io/Main
// https://wiki.qt.io/Qt_for_Beginners
// https://doc.qt.io/qt-6/qtwidgets-index.html
// https://doc.qt.io/qt-6/qpushbutton.html#

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Cnstruct UI, ecs, simulationManager. logConsole
    ui->setupUi(this);
    ecs = new ECS(1, 1);
    simulationManager = new SimulationManager(ecs);

    logConsole = new LogConsole(this);
    logConsole->setParent(ui->logConsole->parentWidget());
    logConsole->setGeometry(ui->logConsole->geometry());
    logConsole->show();
    ui->logConsole->hide();

    // Handle logCOnsole
    connect(ecs, &ECS::ECSLog, logConsole, &LogConsole::appendLog);
    connect(simulationManager, &SimulationManager::simulationLog, logConsole, &LogConsole::appendLog);

    for (int i = 1; i <= ecs->getElevatorCount(); i++) {
        Elevator* elevator = ecs->getElevator(i);
        if (elevator) {
            connect(elevator->getDisplay(), &Display::displayLog,
            this, &MainWindow::logToConsole);
        }
    }

    for (int i = 1; i <= ecs->getElevatorCount(); i++) {
            Elevator* elevator = ecs->getElevator(i);
            if (elevator) {
                connect(elevator->getDoor(), &Door::doorLog,
                        this, &MainWindow::logToConsole);
            }
        }

    // Handlepush buttons
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pauseSimulation);
    connect(ui->continueButton, &QPushButton::clicked, this, &MainWindow::continueSimulation);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);

    connect(ui->addElevatorButton, &QPushButton::clicked, this, &MainWindow::addElevator);
    connect(ui->addFloorButton, &QPushButton::clicked, this, &MainWindow::addFloor);
    connect(ui->addPassengerButton, &QPushButton::clicked, this, &MainWindow::addPassenger);
    //connect(ui->floorButtonTable, &QTableWidget::cellClicked, this, &MainWindow::simulateButtonPress);

    connect(ui->fireButton, &QPushButton::clicked, this, &MainWindow::triggerFireAlert);
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::triggerPowerOut);

    // Setup and update UI
    setupFloorButtons();
    setupCarButtons();
    updateUI();
    updateElevatorStatus();

    // Might unused
    QTimer* uiUpdateTimer = new QTimer(this);
    connect(uiUpdateTimer, &QTimer::timeout, this, &MainWindow::updateElevatorStatus);
    uiUpdateTimer->start(0);

    // Handle combo box for paassenger initialization
    for (int i = 1; i <= ecs->getFloorCount(); i++) {
        ui->startFloorComboBox->addItem(QString::number(i));
        ui->destFloorComboBox->addItem(QString::number(i));
    }

    // For testing
    /*
        Passenger* p1 = new Passenger(1, 3, 5);
        ecs->addPassenger(p1);

        Passenger* p2 = new Passenger(2, 3, 7);
        ecs->addPassenger(p2);

        Passenger* p3 = new Passenger(3, 3, 9);
        ecs->addPassenger(p3);

        SimulationManager* sim = SimulationManager::getInstance(ecs);
        sim->addPassengerAction(1, p1, PassengerAction(PassengerActionType::FloorRequest, 3, 'U'));
        sim->addPassengerAction(1, p2, PassengerAction(PassengerActionType::FloorRequest, 3, 'U'));
        sim->addPassengerAction(1, p3, PassengerAction(PassengerActionType::FloorRequest, 3, 'U'));
        //sim->addPassengerAction(4, p1, PassengerAction(PassengerActionType::CloseDoor, 0));


        //sim->addPassengerAction(2, p1, PassengerAction(PassengerActionType::CarRequest, 5));
    */

}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
    delete ecs;
    delete simulationManager;
    //SimulationManager::destroyInstance();

    if (logConsole) {
        delete logConsole;
        logConsole = nullptr;
    }
}

// Adders
void MainWindow::addElevator() {
    ecs->addElevator();
    setupCarButtons();
    updateUI();
    updateElevatorStatus();
}

void MainWindow::addFloor() {
    ecs->addFloor();
    setupFloorButtons();
    setupCarButtons();
    floorCount++;
    ui->startFloorComboBox->addItem(QString::number(floorCount));
    ui->destFloorComboBox->addItem(QString::number(floorCount));

    setupFloorButtons();
    setupCarButtons();

    updateUI();
    updateElevatorStatus();
}

void MainWindow::addPassenger() {
    int startFloor = ui->startFloorComboBox->currentText().toInt();
    int destFloor = ui->destFloorComboBox->currentText().toInt();
    int timeStep = ui->timeStepSpinBox->value();

    if (startFloor == destFloor) {
        qDebug() << "[mainwindow] passenger start and dest position are same";
        return;
    }

    static int passengerId = 1;

    Passenger* newPassenger = new Passenger(passengerId++, startFloor, destFloor);

    ecs->addPassenger(newPassenger);

    char direction = (destFloor > startFloor) ? 'U' : 'D';

    //SimulationManager* sim = SimulationManager::getInstance(ecs);
    simulationManager->addPassengerAction(timeStep, newPassenger, PassengerAction(PassengerActionType::FloorRequest, startFloor, direction));

    qDebug() << "[MainWindow] Passenger" << newPassenger->getId()
             << "Start floor:" << startFloor
             << "Time step:" << timeStep
             << "Dest floor:" << destFloor
             << "And pressed " << (direction == 'U' ? "UP" : "DOWN") << " button at"
             << startFloor << " floor ";
    passengerCount++;
    updateUI();
}

// Setup and update UI
void MainWindow::updateUI() {
    ui->elevatorCountLabel->setText(QString("Elevator Count: %1").arg(ecs->getElevatorCount()));
    ui->floorCountLabel->setText(QString("Floor Count: %1").arg(ecs->getFloorCount()));
    ui->passengerCountLabel->setText(QString("Passengers: %1").arg(passengerCount));
}

void MainWindow::updateElevatorStatus() {
    ui->elevatorStatusLabel->setText(QString::fromStdString(ecs->getElevatorStatus()));
}

void MainWindow::setupFloorButtons() {
    int floors = ecs->getFloorCount();
    ui->floorButtonTable->setRowCount(floors);
    ui->floorButtonTable->setColumnCount(2);
    ui->floorButtonTable->setHorizontalHeaderLabels({"Up", "Down"});

    for (int i = 0; i < floors; ++i) {
        Floor* floor = ecs->getFloor(i + 1);
        if (!floor) continue;

        QPushButton *upButton = new QPushButton("▲");
        ui->floorButtonTable->setCellWidget(i, 0, upButton);
        connect(upButton, &QPushButton::clicked, [floor]() {
            qDebug() << "[MainWindow] Floor " << floor->getFloorNumber() << " UP button preessed";
            floor->getUpButton()->pressButton();
        });

        QPushButton *downButton = new QPushButton("▼");
        ui->floorButtonTable->setCellWidget(i, 1, downButton);
        connect(downButton, &QPushButton::clicked, [floor]() {
            qDebug() << "[MainWindow] Floor " << floor->getFloorNumber() << " DOWN buttonn presssed";
            floor->getDownButton()->pressButton();
        });
    }
}

void MainWindow::setupCarButtons() {
    int elevators = ecs->getElevatorCount();

    ui->carButtonTable->setRowCount(elevators);
    ui->carButtonTable->setColumnCount(ecs->getFloorCount() + 5);

    // Handle each elevator
    for (int i = 0; i < elevators; ++i) {
        Elevator* elevator = ecs->getElevator(i + 1);

        if (!elevator) continue;

        CarButtonPanel* panel = elevator->getCarButtonPanel();

        for (int j = 0; j < ecs->getFloorCount(); ++j) {
            int floorNumber = j + 1;
            QPushButton* floorButton = new QPushButton(QString::number(floorNumber));
            ui->carButtonTable->setCellWidget(i, j, floorButton);
            connect(floorButton, &QPushButton::clicked, [panel, floorNumber]() {
                qDebug() << "[MainWindow] Elevator internal button for floor " << floorNumber << " pressed.";
                panel->pressButton(floorNumber);
            });
        }

        QPushButton* openButton = new QPushButton("Open");
        ui->carButtonTable->setCellWidget(i, ecs->getFloorCount(), openButton);
        connect(openButton, &QPushButton::clicked, [elevator]() {
        qDebug() << "[MainWindow] Elevator " << elevator->getId() << " Open button pressed.";
        elevator->openDoor();
        });

        QPushButton* closeButton = new QPushButton("Close");
        ui->carButtonTable->setCellWidget(i, ecs->getFloorCount() + 1, closeButton);
        connect(closeButton, &QPushButton::clicked, [elevator]() {
        qDebug() << "[MainWindow] Elevator " << elevator->getId() << " Close button pressed.";
        elevator->closeDoor();
        });

        QPushButton* obstacleButton = new QPushButton("Add Obs");
        ui->carButtonTable->setCellWidget(i, ecs->getFloorCount() + 2, obstacleButton);
        connect(obstacleButton, &QPushButton::clicked, [elevator, obstacleButton]() {
        elevator->getDoor()->addRemoveObstacle();
        obstacleButton->setText(elevator->getDoor()->hasObstacle() ? "Del Obs" : "Add Obs");
        qDebug() << "[MainWindow] Elevator " << elevator->getId()
        << (elevator->getDoor()->hasObstacle() ? "Obstacle Added" : "Obstacle Removed");
        });

        QPushButton* overloadButton = new QPushButton("Set Overload");
        ui->carButtonTable->setCellWidget(i, ecs->getFloorCount() + 3, overloadButton);
        connect(overloadButton, &QPushButton::clicked, [elevator, overloadButton]() {
        elevator->toggleOverload();
        overloadButton->setText(elevator->isOverloaded() ? "Rem Overload" : "Set Overload");
        qDebug() << "[MainWindow] Elevator " << elevator->getId()
        << (elevator->isOverloaded() ? "is overloaded" : "no longer overloaded");
        });

        QPushButton* helpButton = new QPushButton("Help");
        ui->carButtonTable->setCellWidget(i, ecs->getFloorCount() + 4, helpButton);
        QTimer* helpTimer = new QTimer(this);
        helpTimer->setSingleShot(true);

        connect(helpButton, &QPushButton::clicked, [this, elevator, helpButton, helpTimer]() {
        static bool helpRequested = false;

        if (!helpRequested) {
            helpRequested = true;
            helpButton->setText("Cancel Help");
            QString logMsg = QString("[MainWindow] Elevator %1: Help requested.").arg(elevator->getId());
            qDebug() << logMsg;
            logConsole->appendLog(logMsg);

            helpTimer->start(5000);
            connect(helpTimer, &QTimer::timeout, [this, elevator]() {
            QString logMsg = QString("[MainWindow] Elevator %1: Help request forwarded to 911.").arg(elevator->getId());
            qDebug() << logMsg;
            logConsole->appendLog(logMsg);
            });
        } else {
            helpRequested = false;
            helpButton->setText("Help");
            helpTimer->stop();
            QString logMsg = QString("[MainWindow] Elevator %1: Help request canceled.").arg(elevator->getId());
            qDebug() << logMsg;
            logConsole->appendLog(logMsg);
            }
        });
    }
}

// Handle emergency events
void MainWindow::triggerFireAlert() {
    qDebug() << "[MainWindow] Fire alert triggered!";
    ecs->triggerEmergency(EmergencyType::fireAlarm);
}

void MainWindow::triggerPowerOut() {
    qDebug() << "[MainWindow] Power failure triggered!";
    ecs->triggerEmergency(EmergencyType::powerOut);
}


// Handle simulation controller
void MainWindow::startSimulation() {
    qDebug() << "[MainWindow] Simulation started.";
    //SimulationManager::getInstance(ecs)->startSimulation();
    simulationManager->startSimulation();
}

void MainWindow::pauseSimulation() {
    qDebug() << "[MainWindow] Simulation paused.";
    //SimulationManager::getInstance()->pauseSimulation();
    simulationManager->pauseSimulation();
}

void MainWindow::continueSimulation() {
    qDebug() << "[MainWindow] Simulation continued.";
    //SimulationManager::getInstance()->resumeSimulation();
    simulationManager->continueSimulation();
}

void MainWindow::stopSimulation() {
    qDebug() << "[MainWindow] Simulation stopped.";
    //SimulationManager::getInstance()->stopSimulation();
    simulationManager->stopSimulation();
}

// Handle log console
void MainWindow::logToConsole(const QString& message) {
    logConsole->appendLog(message);
}

