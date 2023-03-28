#include "button.h"

void setButtonInterruption(uint8_t INTX ){
    EIMSK |= (1<<INTX);
}

void setButtonAnyEdge(volatile uint8_t* registers, uint8_t pin, uint8_t INTX){
    cli();
    *registers &= ~(1<<pin);
    setButtonInterruption(INTX);
    EICRA |= (1<<ISC00);
    EICRA &= ~(1<<ISC01);
    sei();
}

void setButtonFallingEdge(volatile uint8_t* registers, uint8_t pin, uint8_t INTX){
    cli();
    *registers &= ~(1<<pin);
    setButtonInterruption(INTX);
    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC00);
    sei();
}

void setButtonRisingEdge(volatile uint8_t* registers, uint8_t pin, uint8_t INTX){
    cli();
    *registers &= ~(1<<pin);
    setButtonInterruption(INTX);
    EICRA |= (1<<ISC01) | (1<<ISC00);
    sei();
}

bool isButtonPressed(volatile uint8_t* registers, uint8_t pin){
    if (*registers & (1<<pin)){
        _delay_ms(BUTTON_DEBOUNCE_DELAY);
        return (*registers & (1<<pin));
    }
    return false;
}
