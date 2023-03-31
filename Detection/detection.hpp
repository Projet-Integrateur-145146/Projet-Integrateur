#include "Can/can.h"

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
    uint8_t facingDirection_; // 0 = Top, =1 for each 45 degrees turn [0,7]
    uint8_t positionWithPole_ [32];
    uint8_t currentPosition_;
    Led led_{&PORTA,&DDRA,PA0,PA1};
};

