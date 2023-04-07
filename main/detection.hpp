#ifndef COMMON_H
#define COMMON_H
#include "../common.hpp"
#endif
#include "Can/can.h"
#include "PWM/pwm.h"
#include "Piezo/piezo.h"

#define CONFIRM_INITIAL_DIRECTION 2000
#define LECTURE_DELAY 5
#define Y_DISTANCE 8
#define X_DISTANCE 1
#define NOT_SIGNIFICANT_BITS 2
#define MIN_VALUE_TWO_DIAGONAL 37
#define MAX_VALUE_TWO_DIAGONAL 42
#define MIN_VALUE_TWO_HORIZONTAL 50
#define MAX_VALUE_TWO_HORIZONTAL 70
#define MIN_VALUE_ONE_DIAGONAL 85
#define MAX_VALUE_ONE_DIAGONAL 120
#define MIN_VALUE_ONE_HORIZONTAL 160
#define NORTH 0
#define NORTH_EAST 1
#define EAST 2
#define SOUTH_EAST 3
#define SOUTH 4
#define SOUTH_WEST 5
#define WEST 6
#define NORTH_WEST 7
#define TWO_SPACES_AWAY 2
#define ONE_SPACE_AWAY 1
#define MIN_NUMBER_GOOD_READINGS 2
#define WHEELS_OFF 100
#define WHEELS_FAST 35
#define DELAY_BETWEEN_READINGS 2
#define NUMBER_READINGS_PREVIOUS_FACING_DIRECTION 150
#define NUMBER_READINGS_CURRENT_FACING_DIRECTION 270
#define WHEELS_FAST_LEFT_WHEEL_ADJUSTMENT 30
#define DELAY_BEFORE_CORRECTION_READING 500
#define WHEELS_SLOW 70
#define DELAY_BEFORE_OTHER_READING 1
#define DELAY_PREVENT_CAR_SLIDING 500
#define DELAY_BETWEEN_TWO_TURNS 1000
#define WHEELS_HALF_SPEED 60
#define NO_POLE_FOUND 0

class Detection{
public:
    Detection();
    void executeDetectionState();
    void waitFacingDirection();
    void searchPole();
    void savePole(uint8_t);
    uint8_t findPolePosition();
    bool findPole();
    bool turn45Right();
    void moveToPole();
    void writePolesInMemory();
    void declareFinish();
private:
    uint8_t nbPoles_;
    uint8_t facingDirection_; 
    uint8_t positionsWithPole_ [32];
    uint8_t currentPosition_;
    uint8_t numberOfGoodReadings_;
    uint8_t maxValueRead_;
    bool hasFoundNoPole_;
    Led led_{&PORTA,&DDRA,PA0,PA1};
    PWM wheels_{};
    can can_;
    Piezo piezo_{};
};

void startDetecting();

