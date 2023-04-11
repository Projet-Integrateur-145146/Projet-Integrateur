//#include "../Detection/detection.hpp"
#include "transmission.h"
#include "detection.hpp"

enum class Robot{INIT,DETECTION,TRANSMISSION};
Led led{&PORTB,&DDRB,PB2,PA3};
Timer timer; 

volatile bool ledOn = false; 

ISR(TIMER1_COMPA_vect) {
    if (!ledOn) {
        ledOn = true; 
        led.turnLedGreen(); 
    }
    else {
        ledOn = false; 
        led.turnLedOff(); 
    }
}

void executeTransmission(Transmission& trans) {
    led.turnLedGreen();
    _delay_ms(2000); 
    led.turnLedOff();  
    sei(); 
    timer.timerOneCTC1024(390); 
    trans.generateSVG();
    cli();
    led.turnLedOff(); 
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
        case Robot::TRANSMISSION: stateRobot = Robot::INIT; break; 
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
}