//#include "../Detection/detection.hpp"
#include "../common.hpp"
#include "transmission.h"

enum class Robot{INIT,DETECTION,TRANSMISSION};


void executeTransmission() {
    Transmission transmission; 
    transmission.generateSVG(); 
}
void executeDetection() {
    // Code pour execute la classe de detection 
}

void executeINIT() {return ;}


// INTERRUPT = PC4
// BLANC = PC6
void switchState(Robot& stateRobot) {
    switch (stateRobot) {
        case Robot::INIT: 
            if (isButtonPressed(&PINC,PINC4))
                stateRobot = Robot::DETECTION;  
            else if (!(isButtonPressed(&PINC,PINC6))) 
                stateRobot = Robot::TRANSMISSION;  
            break; 
        case Robot::DETECTION: break; 
        case Robot::TRANSMISSION: break; 
    }   
}

void executeState(Robot& stateRobot) {
    switch (stateRobot) {
        case Robot::INIT: executeINIT(); break; 
        case Robot::DETECTION: executeDetection(); break; 
        case Robot::TRANSMISSION: executeTransmission(); break; 
    }
}

int main(){

    Robot robot = Robot::INIT;
    DDRC &= ~(1<<PC4) & ~(1<<PC6);

    while (true) {
        switchState(robot);
        executeState(robot);
    }
    Led led {&PORTA,&DDRA,PA0,PA1};
    led.turnLedRed();
}
