#include "../common.hpp"
#include "detection.hpp"

#define CONFIRM_INITIAL_DIRECTION 2000

void Detection::waitFacingDirection(){
    Led led {&PORTA,&DDRA,PA0,PA1};
    while (true){
        led.turnLedAmber();
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            facingDirection_ = 0;
            led.turnLedGreen();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            facingDirection_ = 2;
            led.turnLedRed();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
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