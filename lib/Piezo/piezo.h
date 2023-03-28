#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/twi.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Debug/debug.h>
#define DELAY_AMBER 5

class Piezo {
public:
    Piezo() {
        DDRD |= (1 << DD6) | (1 << DD7);
        PORTD &= ~(1 << PORTD6);
        TCCR2B |= (1 << CS22);
        TCCR2A |= (1 << WGM21) | (1 << COM2A0) ;
    }

    void playSound(uint8_t note);

    void stopSound();
private:
    float frequency;
};