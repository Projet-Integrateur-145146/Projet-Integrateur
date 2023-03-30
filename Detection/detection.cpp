#include "../common.hpp"
#include "detection.hpp"

void Detection::waitFacingDirection(){
    Led led {&PORTA,&DDRA,PA0,PA1};
    while (true){
        led.turnLedAmber();
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            initialDirection_ = 0;
            break;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            initialDirection_ = 1;
            break;
        }
    }
    led.turnLedOff();
}

void Detection::executeDetectionState(){
    waitFacingDirection();
};

Detection::Detection(){
    nbPoles_ = 0;
};

int main(){
    DDRC &= ~(1<<PC4) & ~(1<<PC6);
    Detection detect;
    detect.executeDetectionState();
}