#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>




class Timer 
{
public:
    Timer(); 
    ~Timer(); 

    // Initialise un timer en mode CTC avec un prescaler de 1024 
    void timerOneCTC1024(uint16_t duration);
    
    // Initialise un timer en mode CTC avec un prescaler de 8 
    void timerOneCTC8(uint16_t duration); 

    // Initialise un timer en mode FastPWM avec un prescaler de 1024
    void timerOneFastPWM1024(uint16_t duration);

    // Initialise un timer en mode FastPWM avec un prescaler de 8
    void timerOneFastPWM8(uint16_t duration); 

    // Initialise le timer 2 en mode CTC avec un prescaler de 1024 
    void timerTwoCTC1024(uint8_t duration);
    
    // Initialise le timer 2 en mode CTC avec un prescaler de 8 
    void timerTwoCTC8(uint8_t duration); 

    // Initialise le timer 2 en mode FastPWM avec un prescaler de 1024
    void timerTwoFastPWM1024(uint8_t duration);

    // Initialise le timer 2 en mode FastPWM avec un prescaler de 8
    void timerTwoFastPWM8(uint8_t duration);     
};