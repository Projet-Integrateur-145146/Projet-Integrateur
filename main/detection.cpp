#include "detection.hpp"

volatile uint8_t gButtonPressed = 0;

ISR (INT2_vect) {
_delay_ms ( 30 );

if (PINB & (1<<PINB2)){
    gButtonPressed = 1;
}
EIFR |= (1 << INTF2) ;
}

Detection::Detection() : positionsWithPole_{}{
    nbPoles_ = 0;
    currentPosition_ = 0;
    numberOfGoodReadings_ = 0;
    hasFoundNoPole_ = false;
    DDRA &= ~(1 << PA3); // configure le port PD3 en mode entrée
};

void Detection::waitFacingDirection(){
    while (true){
        led_.turnLedAmber();
        if (isButtonPressed(&PINB,PINB2)){ // Bouton Interrupt
            facingDirection_ = NORTH;
            led_.turnLedGreen();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }
        if (!(isButtonPressed(&PINC,PINC6))){ // Blanc
            facingDirection_ = EAST;
            led_.turnLedRed();
            _delay_ms(CONFIRM_INITIAL_DIRECTION);
            break;
        }
    }
    led_.turnLedOff();
}

void Detection::savePole(uint8_t poleDistance){
    uint8_t tempPosition = -1;
    switch(facingDirection_){
        case NORTH:
            tempPosition = currentPosition_+Y_DISTANCE*poleDistance;
            break;
        case NORTH_EAST:
            tempPosition = currentPosition_+(Y_DISTANCE+X_DISTANCE)*poleDistance;
            break;
        case EAST:
            tempPosition = currentPosition_+X_DISTANCE*poleDistance;
            break;
        case SOUTH_EAST:
            tempPosition = currentPosition_+(X_DISTANCE-Y_DISTANCE)*poleDistance;
            break;
        case SOUTH:
            tempPosition = currentPosition_-(Y_DISTANCE*poleDistance);
            break;
        case SOUTH_WEST:
            tempPosition = currentPosition_-(X_DISTANCE+Y_DISTANCE)*poleDistance;
            break;
        case WEST:
            tempPosition = currentPosition_-(X_DISTANCE*poleDistance);
            break;
        case NORTH_WEST:
            tempPosition = currentPosition_+(Y_DISTANCE-X_DISTANCE)*poleDistance;
            break;
    }
    if ((tempPosition > 32) || tempPosition < 0) {return;}
    positionsWithPole_[tempPosition] = 1;
    currentPosition_ = tempPosition;
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
        if (value < MIN_VALUE_TWO_DIAGONAL){
            return 0;
        }

        if((value >= MIN_VALUE_TWO_DIAGONAL) && (value <= MAX_VALUE_TWO_DIAGONAL)){
            // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            maxValueRead_ = value;
            return TWO_SPACES_AWAY;
            led_.turnLedGreen();
        }
        else if((value >= MIN_VALUE_TWO_HORIZONTAL) && (value <= MAX_VALUE_TWO_HORIZONTAL)){
            // sprintf(buff,"Robot à 2 poteaux tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            maxValueRead_ = value;
            return TWO_SPACES_AWAY;
            
            led_.turnLedOff();
        }
        else if((value >= MIN_VALUE_ONE_HORIZONTAL)){
            // sprintf(buff,"Robot à 1 poteau tout droit%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            maxValueRead_ = value;
            return ONE_SPACE_AWAY;
            led_.turnLedOff();
        }
        else if((value >= MIN_VALUE_ONE_DIAGONAL) && (value <= MAX_VALUE_ONE_DIAGONAL)){
            // sprintf(buff,"Robot à 1 poteau diagonale%u\n",value);
            // const char* str_pot = buff;
            // printDebug(str_pot);
            maxValueRead_ = value;
            return ONE_SPACE_AWAY;
            led_.turnLedRed();
        }

        else{
            return NO_POLE_FOUND;
            led_.turnLedOff();
            }
       // }

        // printDebug(space);
        // sprintf(buff,"Robot à 2 poteaux diagonale %u\n",value);
        // const char* str_pot = buff;
        // printDebug(str_pot);

}

bool Detection::findPole(){
    uint16_t value;
    uint8_t pos = PA3;
    value = can_.lecture(pos);
    value = value >> NOT_SIGNIFICANT_BITS;
    if ((value >= MIN_VALUE_TWO_DIAGONAL)){
        numberOfGoodReadings_++;
    }

    else{
        return false;
    }
    
    if (numberOfGoodReadings_ >= MIN_NUMBER_GOOD_READINGS){
        return true;
    }
    
    return ((value >= MIN_VALUE_TWO_DIAGONAL) && findPole());
}

bool Detection::turn45Right(){
    uint16_t numberOfTimes = 0;
    wheels_.setBackwardRight();
    wheels_.setForwardLeft();
    wheels_.ajustPWM(WHEELS_FAST,WHEELS_FAST);
    while (numberOfTimes < NUMBER_READINGS_PREVIOUS_FACING_DIRECTION){
        numberOfGoodReadings_ = 0;
        if (findPole()){
            wheels_.ajustPWM(WHEELS_OFF, WHEELS_OFF);
            return true;
        }
        _delay_ms(DELAY_BETWEEN_READINGS);
        numberOfTimes++;
    }

    if (facingDirection_ == NORTH_WEST){
        facingDirection_ = NORTH;
    }

    else{
        facingDirection_++;
    }
    
    while (numberOfTimes < NUMBER_READINGS_CURRENT_FACING_DIRECTION){
        numberOfGoodReadings_ = 0;
        if (findPole()){
            wheels_.ajustPWM(WHEELS_OFF, WHEELS_OFF);
            return true;
        }
        _delay_ms(DELAY_BETWEEN_READINGS);
        numberOfTimes++;
    }
    
    //_delay_ms(500);
    wheels_.ajustPWM(WHEELS_OFF, WHEELS_OFF);
    return false;
}

void Detection::moveToPole(){
    uint16_t value = can_.lecture(PA3);
    value = value >> NOT_SIGNIFICANT_BITS;
    if (value >= MIN_VALUE_ONE_HORIZONTAL){
        return;
    }

    wheels_.setForwardAll();
    wheels_.ajustPWM(WHEELS_FAST,WHEELS_FAST);
    _delay_ms(DELAY_BEFORE_CORRECTION_READING);
    value = can_.lecture(PA3);
    value = value >> NOT_SIGNIFICANT_BITS;
    uint16_t increment = 0;
    while ((value+1 < maxValueRead_) && (increment < 1250) ){
        if (increment < 500){
           wheels_.setBackwardLeft();
            wheels_.setForwardRight();
            wheels_.ajustPWM(WHEELS_SLOW,WHEELS_SLOW);
            _delay_ms(1);
            value = can_.lecture(PA3);
            value = value >> NOT_SIGNIFICANT_BITS;
            wheels_.ajustPWM(WHEELS_OFF,WHEELS_OFF); 
        }
        else{
            wheels_.setBackwardRight();
            wheels_.setForwardLeft();
            wheels_.ajustPWM(WHEELS_SLOW,WHEELS_SLOW);
            _delay_ms(1);
            value = can_.lecture(PA3);
            value = value >> NOT_SIGNIFICANT_BITS;
            wheels_.ajustPWM(WHEELS_OFF,WHEELS_OFF); 
        }
        increment++;
        
    }
    _delay_ms(DELAY_PREVENT_CAR_SLIDING);


    if (value > maxValueRead_){
        maxValueRead_ = value;
    }
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
        _delay_ms(DELAY_BETWEEN_TWO_TURNS);
    }

    if (!isTherePole){
        isTherePole = turn45Right();
        if (!isTherePole){
            hasFoundNoPole_ = true;
            return;
        }
        facingDirection_ = stopTurningFacingDirection;
    }
    
    // Trouve Pole - Doit retourner la distance au pole (1 ou 2)
    uint8_t poleDistance = findPolePosition();
    // while (poleDistance == NO_POLE_FOUND){
    //     wheels_.setBackwardLeft();
    //     wheels_.setForwardRight();
    //     wheels_.ajustPWM(WHEELS_HALF_SPEED,WHEELS_HALF_SPEED);
    //     _delay_ms(DELAY_BEFORE_OTHER_READING);
    //     poleDistance = findPolePosition();
    // }
    wheels_.ajustPWM(WHEELS_OFF,WHEELS_OFF);
    
    //uint16_t value = can_.lecture(PA3);
    //maxValueRead_ = value >> NOT_SIGNIFICANT_BITS;
    savePole(poleDistance); // Sauvegarde position Pole
    // Avance vers Pole
    uint8_t timesMoved = 0;
    do{
        moveToPole();
        led_.turnLedAmber();
        timesMoved++;
    } while ((maxValueRead_ < MIN_VALUE_ONE_HORIZONTAL) && (timesMoved <= 200));
        
        
        // if (timesMoved > 200){
        //     break;
        // }>
        //timesMoved++;
    
    wheels_.ajustPWM(WHEELS_OFF,WHEELS_OFF);
    for (uint8_t i = 0; i<3;i++){
        piezo_.playSound(81);
        _delay_ms(300);
        piezo_.stopSound();
        _delay_ms(300);
    }

    return;

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
    EIMSK |= (1 << INT2);
    while(!hasFoundNoPole_){
        led_.turnLedAmber();
        if(gButtonPressed){
            facingDirection_ = NORTH;
            EIMSK &= ~(1 << INT2);
            searchPole();
            gButtonPressed = 0;
            EIMSK |= (1 << INT2);
        }
    }
    //findPolePosition();
};

void Detection::writePolesInMemory(){
    Memoire24CXXX mem;
    uint8_t* ptr = positionsWithPole_;
    mem.ecriture(0,ptr,32);
}

void Detection::declareFinish(){
    piezo_.playSound(45);
    _delay_ms(2000);
    piezo_.stopSound();
    while(true){
        led_.turnLedRed();
        _delay_ms(250);
        led_.turnLedOff();
        _delay_ms(250);
        led_.turnLedRed();
        _delay_ms(250);
        led_.turnLedOff();
        _delay_ms(250);
    }
}

void initialisation(void){
cli ();
DDRC &= ~(1<<PB2) & ~(1<<PC6);
EICRA |= (1<<ISC20) | (1<<ISC21);   
sei ();
}

void startDetecting(){
    initialisation();
    Detection detect;
    //detect.findPolePosition();
    detect.executeDetectionState();
    detect.writePolesInMemory();
    detect.declareFinish();
    // PWM wheels;
    // wheels.setBackwardRight();
    // wheels.setForwardLeft();
    // wheels.ajustPWM(WHEELS_FAST,WHEELS_FAST);
}