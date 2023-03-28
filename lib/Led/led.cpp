#include "led.hpp"


Led::Led(volatile uint8_t* registers, volatile uint8_t* DDRX, uint8_t port1, uint8_t port2){
    register_ = registers;
    DDRX_ = DDRX;
    PORT0_ = port1;
    PORT1_ = port2;

    *DDRX_ |= (1<<PORT0_) | (1<<PORT1_);
}
void Led::turnLedGreen(){
    *register_ &= ~(1 << PORT1_);
    *register_ |= (1 << PORT0_);
}
void Led::turnLedRed(){
    *register_ &= ~(1 << PORT0_);
    *register_ |= (1 << PORT1_);
}
void Led::turnLedAmber(){
    _delay_ms(DELAY_AMBER);
    turnLedGreen();
    _delay_ms(DELAY_AMBER);
    turnLedRed();
}
void Led::turnLedOff(){
    *register_ &= ~(1 << PORT0_);
    *register_ &= ~(1 << PORT1_);
}
