#include "main.hpp"
//#include "../Detection/detection.hpp"
#include "../common.hpp"

// INTERRUPT = PC4
// BLANC = PC6

enum class Robot{INIT,DETECTION,TRANSMISSION};

Robot detectState(){
    Robot robot = Robot::INIT;
    DDRC &= ~(1<<PC4) & ~(1<<PC6);
    while (robot == Robot::INIT){
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            robot = Robot::DETECTION;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            robot = Robot::TRANSMISSION;
        }
    }
    return robot;
}



void executeTransmissionState(){

}

int main(){
    Robot robot = detectState();
    Led led {&PORTA,&DDRA,PA0,PA1};
    led.turnLedRed();
    // if (robot == Robot::DETECTION){
    //     Detection detect;
    //     detect.executeDetectionState();
    // }
    // else{
    //     executeTransmissionState();
    // }

}