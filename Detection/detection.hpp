#include "../common.hpp"
#include "Can/can.h"
#include "PWM/pwm.h"

#define CONFIRM_INITIAL_DIRECTION 2000
#define LECTURE_DELAY 5
#define Y_DISTANCE 8
#define X_DISTANCE 1
#define NOT_SIGNIFICANT_BITS 2
#define MIN_VALUE_TWO_DIAGONAL 18
#define MAX_VALUE_TWO_DIAGONAL 23
#define MIN_VALUE_TWO_HORIZONTAL 25
#define MAX_VALUE_TWO_HORIZONTAL 29
#define MIN_VALUE_ONE_DIAGONAL 46
#define MAX_VALUE_ONE_DIAGONAL 50
#define MIN_VALUE_ONE_HORIZONTAL 90
#define NORTH 0
#define NORTH_EAST 1
#define EAST 2
#define SOUTH_EAST 3
#define SOUTH 4
#define SOUTH_WEST 5
#define WEST 6
#define NORTH_WEST 7

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
private:
    uint8_t nbPoles_;
    uint8_t facingDirection_; 
    uint8_t positionsWithPole_ [32];
    uint8_t currentPosition_;
    Led led_{&PORTA,&DDRA,PA0,PA1};
    PWM wheels_{};
    can can_;
};

