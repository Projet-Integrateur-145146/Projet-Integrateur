#include "detection.hpp"

Detection::Detection() : positionsWithPole_{}{
    nbPoles_ = 0;
    currentPosition_ = 0;
    numberOfGoodReadings_ = 0;
    DDRA &= ~(1 << PA3); // configure le port PD3 en mode entrée
};

void Detection::waitFacingDirection(){
    while (true){
        led_.turnLedAmber();
        if (isButtonPressed(&PINC,PINC4)){ // Bouton Interrupt
            facingDirection_ = NORTH;
            led_.turnLedGreen();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }

        else if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            facingDirection_ = EAST;
            led_.turnLedRed();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }
    }
    led_.turnLedOff();
}

void Detection::savePole(uint8_t poleDistance){
    switch(facingDirection_){
        case NORTH:
            positionsWithPole_[currentPosition_+Y_DISTANCE*poleDistance] = 1;
            break;
        case NORTH_EAST:
            positionsWithPole_[currentPosition_+(Y_DISTANCE+X_DISTANCE)*poleDistance] = 1;
            break;
        case EAST:
            positionsWithPole_[currentPosition_+X_DISTANCE*poleDistance] = 1;
            break;
        case SOUTH_EAST:
            positionsWithPole_[currentPosition_+(X_DISTANCE-Y_DISTANCE)*poleDistance] = 1;
            break;
        case SOUTH:
            positionsWithPole_[currentPosition_-(Y_DISTANCE*poleDistance)] = 1;
            break;
        case SOUTH_WEST:
            positionsWithPole_[currentPosition_-(X_DISTANCE+Y_DISTANCE)*poleDistance] = 1;
            break;
        case WEST:
            positionsWithPole_[currentPosition_-(X_DISTANCE*poleDistance)] = 1;
            break;
        case NORTH_WEST:
            positionsWithPole_[currentPosition_+(Y_DISTANCE-X_DISTANCE)*poleDistance] = 1;
            break;
    }
    nbPoles_++;

}

uint8_t Detection::findPolePosition(){
    uint16_t value;
    uint8_t pos = PA3;
    // char space[2] = " ";
    //while(true){
        value = can_.lecture(pos);
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
            //led_.turnLedRed();
        }
        else if((value >= MIN_VALUE_TWO_HORIZONTAL) && (value <= MAX_VALUE_TWO_HORIZONTAL)){
            // sprintf(buff,"Robot à 2 poteaux tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 2;
            //led_.turnLedOff();
        }
        else if((value >= MIN_VALUE_ONE_HORIZONTAL)){
            // sprintf(buff,"Robot à 1 poteau tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
            //led_.turnLedOff();
        }
        else if((value >= MIN_VALUE_ONE_DIAGONAL) && (value <= MAX_VALUE_ONE_DIAGONAL)){
            // sprintf(buff,"Robot à 1 poteau diagonale%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            return 1;
            //led_.turnLedOff();
        }

        return 0;
        // printDebug(space);
        // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
        // const char* str_pot = buff;
        // printDebug(str_pot);
    //}
}

bool Detection::findPole(){
    uint16_t value;
    uint8_t pos = PA3;
    value = can_.lecture(pos);
    value = value >> NOT_SIGNIFICANT_BITS;
    if (value >= MIN_VALUE_TWO_DIAGONAL){
        numberOfGoodReadings_++;
    }

    else{
        return false;
    }
    
    if (numberOfGoodReadings_ >= 2){
        return true;
    }
    
    return ((value >= MIN_VALUE_TWO_DIAGONAL) && findPole());
}

bool Detection::turn45Right(){
    uint16_t numberOfTimes = 0;
    wheels_.setBackwardRight();
    wheels_.setForwardLeft();
    wheels_.ajustPWM(35,35);
    while (numberOfTimes < 150){
        numberOfGoodReadings_ = 0;
        if (findPole()){
            wheels_.ajustPWM(100,100);
            return true;
        }
        _delay_ms(2);
        numberOfTimes++;
    }

    if (facingDirection_ == NORTH_WEST){
        facingDirection_ = NORTH;
    }

    else{
        facingDirection_++;
    }
    
    while (numberOfTimes < 270){
        numberOfGoodReadings_ = 0;
        if (findPole()){
            wheels_.ajustPWM(100,100);
            return true;
        }
        _delay_ms(2);
        numberOfTimes++;
    }
    
    //_delay_ms(500);
    wheels_.ajustPWM(100,100);
    return false;
}

void Detection::searchPole(){
    // TODO
    bool isTherePole = false;
    uint8_t stopTurningFacingDirection;
    if (facingDirection_ == NORTH){
        stopTurningFacingDirection = NORTH_WEST;
    }
    else{
        stopTurningFacingDirection = NORTH_EAST;
    }
    //uint8_t stopTurningFacingDirection = facingDirection_ - 1;
    while((facingDirection_ != stopTurningFacingDirection) && !isTherePole){// Tourne
        isTherePole = turn45Right();
        _delay_ms(1000);
    }

    if (!isTherePole){
        isTherePole = turn45Right();
        if (!isTherePole){
            return;
        }

        facingDirection_ = stopTurningFacingDirection;
    }
    
    // Trouve Pole - Doit retourner la distance au pole (1 ou 2)
    uint8_t poleDistance = findPolePosition();
    while (poleDistance == 0){
        wheels_.setBackwardLeft();
        wheels_.setForwardRight();
        wheels_.ajustPWM(75,75);
        _delay_ms(1);
        poleDistance = findPolePosition();
    }
    wheels_.ajustPWM(100,100);
    
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
    //detect.executeDetectionState();
    detect.executeDetectionState();
    //detect.findPolePosition();
}