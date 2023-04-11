//#include "../Detection/detection.hpp"
#include "transmission.h"
#include "detection.hpp"

enum class Robot{INIT,DETECTION,TRANSMISSION};


void executeTransmission(Transmission& trans) {
    trans.generateSVG(); 
}
void executeDetection() {
    // Code pour execute la classe de detection 
    startDetecting();
}

void executeINIT() {return ;}


// INTERRUPT = PC4
// BLANC = PC6
void switchState(Robot& stateRobot) {
    switch (stateRobot) {
        case Robot::INIT: 
            if (isButtonPressed(&PINB,PINB2)) {
                while(isButtonPressed(&PINB,PINB2)){}
                stateRobot = Robot::DETECTION;
            }
            else if (!(isButtonPressed(&PINC,PINC6))) {
                while(!(isButtonPressed(&PINC,PINC6))){}
                stateRobot = Robot::TRANSMISSION; 
            } 
            break; 
        case Robot::DETECTION: break; 
        case Robot::TRANSMISSION: break; 
    }
}

void executeState(Robot& stateRobot, Transmission& trans) {
    switch (stateRobot) {
        case Robot::INIT: executeINIT(); break; 
        case Robot::DETECTION: executeDetection(); break; 
        case Robot::TRANSMISSION: executeTransmission(trans); break; 
    }
}

int main(){

    Robot robot = Robot::INIT;
    DDRC &= ~(1<<PB2) & ~(1<<PC6);

    Transmission trans; 

    while (true) {
        switchState(robot);
        executeState(robot, trans);
    }
    Led led {&PORTA,&DDRA,PA0,PA1};
    led.turnLedRed();
}