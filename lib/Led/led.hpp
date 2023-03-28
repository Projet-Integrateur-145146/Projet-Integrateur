#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h> 
#include <util/delay.h>
#define DELAY_AMBER 5

class Led{
    public:
        // Objet led qui enable les port en output avec des options pour changer l'etat de la led
        Led(volatile uint8_t* registers, volatile uint8_t* DDRX, uint8_t port1, uint8_t port2);
        void turnLedGreen();
        void turnLedRed();
        void turnLedAmber();
        void turnLedOff();
        ~Led(){};
    private:
        volatile uint8_t* register_;
        volatile uint8_t* DDRX_;
        uint8_t PORT0_;
        uint8_t PORT1_;
};
