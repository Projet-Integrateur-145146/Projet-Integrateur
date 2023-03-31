#include "../common.hpp"
#include "detection.hpp"

#define CONFIRM_INITIAL_DIRECTION 2000
#define LECTURE_DELAY 5
#define Y_DISTANCE 8
#define X_DISTANCE 1

Detection::Detection() : positionWithPole_{}{
    nbPoles_ = 0;
    currentPosition_ = 0;
};

void Detection::waitFacingDirection(){
    while (true){
        led_.turnLedAmber();
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            facingDirection_ = 0;
            led_.turnLedGreen();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            facingDirection_ = 2;
            led_.turnLedRed();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }
    }
    led_.turnLedOff();
}

void Detection::savePole(uint8_t poleDistance){
    switch(facingDirection_){
        case 0:
            positionWithPole_[currentPosition_+Y_DISTANCE*poleDistance] = 1;
            break;
        case 1:
            positionWithPole_[currentPosition_+(Y_DISTANCE+X_DISTANCE)*poleDistance] = 1;
            break;
        case 2:
            positionWithPole_[currentPosition_+X_DISTANCE*poleDistance] = 1;
            break;
        case 3:
            positionWithPole_[currentPosition_+(X_DISTANCE-Y_DISTANCE)*poleDistance] = 1;
            break;
        case 4:
            positionWithPole_[currentPosition_-(Y_DISTANCE*poleDistance)] = 1;
            break;
        case 5:
            positionWithPole_[currentPosition_-(X_DISTANCE+Y_DISTANCE)*poleDistance] = 1;
            break;
        case 6:
            positionWithPole_[currentPosition_-(X_DISTANCE*poleDistance)] = 1;
            break;
        case 7:
            positionWithPole_[currentPosition_+(Y_DISTANCE-X_DISTANCE)*poleDistance] = 1;
            break;
    }
    nbPoles_++;

}

uint8_t Detection::findPole(){
    can can;
    uint16_t value;
    uint8_t pos = 3;
    // char space[2] = " ";
    DDRA &= ~(1 << PA3); // configure le port PD3 en mode entrée
    while(true){
        _delay_ms(LECTURE_DELAY);
        value = can.lecture(pos);
        value = value >> 2 ;
        //char buffer[6]; // As uint16_t is maximum 5 characters, plus one for the null terminator
        char buff[100];
        // sprintf(buffer, "%u ", value);
        // const char* str_value = buffer;
        // _delay_ms(WAIT);
        // printDebug(str_value);
        _delay_ms(10);
        if((value > 15) && (value < 18 )){
            // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 2;
        }
        else if((value > 23) && (value < 27)){
            // sprintf(buff,"Robot à 2 poteaux tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 2;
        }
        else if((value > 90)){
            // sprintf(buff,"Robot à 1 poteau tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
        }
        else if((value > 46) && (value < 50)){
            // sprintf(buff,"Robot à 1 poteau diagonale%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
        }
        // printDebug(space);
        // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
        // const char* str_pot = buff;
        // printDebug(str_pot);
    }
}

void Detection::searchPole(){
    // TODO
    // Tourne
    uint8_t poleDistance = findPole();
    savePole(poleDistance);
    // Trouve Pole - Doit retourner la distance au pole (1 ou 2)
    // Sauvegarde position Pole
    // Avance vers Pole
    char buffer[6];
    for (uint8_t i = 0; i<32; i++){
        sprintf(buffer, "%u ", positionWithPole_[i]);
        const char* str_value = buffer;
        _delay_ms(10);
        printDebug(str_value);
        _delay_ms(10);
    }
}

void Detection::executeDetectionState(){
    waitFacingDirection();
    searchPole();
};

int main(){
    DDRC &= ~(1<<PC4) & ~(1<<PC6);
    Detection detect;
    detect.executeDetectionState();
}