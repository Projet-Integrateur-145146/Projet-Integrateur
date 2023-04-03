#include "detection.hpp"

Detection::Detection() : positionsWithPole_{}{
    nbPoles_ = 0;
    currentPosition_ = 0;
};

void Detection::waitFacingDirection(){
    while (true){
        led_.turnLedAmber();
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            facingDirection_ = FacingDirection::NORTH;
            led_.turnLedGreen();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            facingDirection_ = FacingDirection::EAST;
            led_.turnLedRed();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }
    }
    led_.turnLedOff();
}

void Detection::savePole(uint8_t poleDistance){
    switch(facingDirection_){
        case FacingDirection::NORTH:
            positionsWithPole_[currentPosition_+Y_DISTANCE*poleDistance] = 1;
            break;
        case FacingDirection::NORTH_EAST:
            positionsWithPole_[currentPosition_+(Y_DISTANCE+X_DISTANCE)*poleDistance] = 1;
            break;
        case FacingDirection::EAST:
            positionsWithPole_[currentPosition_+X_DISTANCE*poleDistance] = 1;
            break;
        case FacingDirection::SOUTH_EAST:
            positionsWithPole_[currentPosition_+(X_DISTANCE-Y_DISTANCE)*poleDistance] = 1;
            break;
        case FacingDirection::SOUTH:
            positionsWithPole_[currentPosition_-(Y_DISTANCE*poleDistance)] = 1;
            break;
        case FacingDirection::SOUTH_WEST:
            positionsWithPole_[currentPosition_-(X_DISTANCE+Y_DISTANCE)*poleDistance] = 1;
            break;
        case FacingDirection::WEST:
            positionsWithPole_[currentPosition_-(X_DISTANCE*poleDistance)] = 1;
            break;
        case FacingDirection::NORTH_WEST:
            positionsWithPole_[currentPosition_+(Y_DISTANCE-X_DISTANCE)*poleDistance] = 1;
            break;
    }
    nbPoles_++;

}

uint8_t Detection::findPole(){
    can can;
    uint16_t value;
    uint8_t pos = PA3;
    // char space[2] = " ";
    DDRA &= ~(1 << PA3); // configure le port PD3 en mode entrée
    //while(true){
        _delay_ms(LECTURE_DELAY);
        value = can.lecture(pos);
        value = value >> NOT_SIGNIFICANT_BITS ;
        //char buffer[6]; // As uint16_t is maximum 5 characters, plus one for the null terminator
        //char buff[100];
        // sprintf(buffer, "%u ", value);
        // const char* str_value = buffer;
        // _delay_ms(WAIT);
        // printDebug(str_value);
        //_delay_ms(10);
        if((value >= MIN_VALUE_TWO_DIAGONAL) && (value <= MAX_VALUE_TWO_DIAGONAL)){
            // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 2;
        }
        else if((value >= MIN_VALUE_TWO_HORIZONTAL) && (value <= MAX_VALUE_TWO_HORIZONTAL)){
            // sprintf(buff,"Robot à 2 poteaux tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 2;
        }
        else if((value >= MIN_VALUE_ONE_HORIZONTAL)){
            // sprintf(buff,"Robot à 1 poteau tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
        }
        else if((value >= MIN_VALUE_ONE_DIAGONAL) && (value <= MAX_VALUE_ONE_DIAGONAL)){
            // sprintf(buff,"Robot à 1 poteau diagonale%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
        }

        return 0;
        // printDebug(space);
        // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
        // const char* str_pot = buff;
        // printDebug(str_pot);
    //}
}

void Detection::turn45Right(){
    wheels_.setBackwardRight();
    wheels_.setForwardLeft();
    wheels_.ajustPWM(230,230);
}

void Detection::searchPole(){
    // TODO
    while(facingDirection_ != FacingDirection::NORTH_WEST){// Tourne
        turn45Right();
    }
    
    uint8_t poleDistance = findPole(); // Trouve Pole - Doit retourner la distance au pole (1 ou 2) Prends 5 ms
    savePole(poleDistance); // Sauvegarde position Pole
    // Avance vers Pole


    // char buffer[6];
    // for (uint8_t i = 0; i<32; i++){
    //     sprintf(buffer, "%u ", positionsWithPole_[i]);
    //     const char* str_value = buffer;
    //     _delay_ms(10);
    //     printDebug(str_value);
    //     _delay_ms(10);
    // }
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