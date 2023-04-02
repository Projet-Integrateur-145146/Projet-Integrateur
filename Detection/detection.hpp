#include "../common.hpp"
#include "Can/can.h"

#define CONFIRM_INITIAL_DIRECTION 2000
#define LECTURE_DELAY 5
#define Y_DISTANCE 8
#define X_DISTANCE 1
#define NOT_SIGNIFICANT_BITS 2
#define MIN_VALUE_TWO_DIAGONAL 15
#define MAX_VALUE_TWO_DIAGONAL 18
#define MIN_VALUE_TWO_HORIZONTAL 23
#define MAX_VALUE_TWO_HORIZONTAL 27
#define MIN_VALUE_ONE_DIAGONAL 46
#define MAX_VALUE_ONE_DIAGONAL 50
#define MIN_VALUE_ONE_HORIZONTAL 90

enum class FacingDirection : uint8_t {NORTH,NORTH_EAST,EAST,SOUTH_EAST,SOUTH,SOUTH_WEST,WEST,NORTH_WEST};

class Detection{
public:
    Detection();
    void executeDetectionState();
    void waitFacingDirection();
    void searchPole();
    void savePole(uint8_t);
    uint8_t findPole();
private:
    uint8_t nbPoles_;
    FacingDirection facingDirection_; 
    uint8_t positionsWithPole_ [32];
    uint8_t currentPosition_;
    Led led_{&PORTA,&DDRA,PA0,PA1};
};

