#include "timer.hpp"

Timer::Timer()
{
    // Ne fait rien (Il va falloir choisir le timer)
}

Timer::~Timer()
{
    // Ne fait rien 
} 

// Initialise un timer en mode CTC avec un prescaler de 1024 
void Timer::timerOneCTC1024(uint16_t duration)
{
    TCNT1 = 0;
    OCR1A = duration;
    TCCR1A |=(1<<COM1A1);// Clear OCR1A (OCnA) On compare match
    TCCR1B |= (1<<CS12)|(1<<CS10)|(1<<WGM12); // WGM12 put timer on CTC mode with OCRnA - CS = 101 to put clk at 1024
    TCCR1C = 0;
    TIMSK1 = (1<<OCIE1A) ; // To set it with OCR1A
}
    
// Initialise un timer en mode CTC avec un prescaler de 8 
void Timer::timerOneCTC8(uint16_t duration)
{
    TCNT1 = 0;
    OCR1A = duration;
    TCCR1A |=(1<<COM1A1);// Clear OCR1A (OCnA) On compare match
    TCCR1B |= (1<<CS11)|(1<<WGM12); // WGM12 put timer on CTC mode with OCRnA - CS = 010 to put clk at 8
    TCCR1C = 0;
    TIMSK1 = (1<<OCIE1A) ; // To set it with OCR1A     
} 

// Initialise un timer en mode FastPWM avec un prescaler de 1024
void Timer::timerOneFastPWM1024(uint16_t duration)
{
    TCNT1 = 0;
    OCR1A = duration;
    TCCR1A |= (1<<WGM10) | (1<<COM1A1);// Clear OCR1A (OCnA) On compare match and WGM10 put timer on FASTPWM mode
    TCCR1B |= (1<<CS12)|(1<<CS10)|(1<<WGM12); // WGM12 put timer on FASTPWM mode with OCRnA - CS = 101 to put clk at 1024
    TCCR1C = 0;
    TIMSK1 = (1<<OCIE1A) ; // To set it with OCR1A
}

// Initialise un timer en mode FastPWM avec un prescaler de 8
void Timer::timerOneFastPWM8(uint16_t duration)
{
    TCNT1 = 0;
    OCR1A = duration;
    TCCR1A |= (1<<WGM10) | (1<<COM1A1);// Clear OCR1A (OCnA) On compare match and WGM10 put timer on FASTPWM mode
    TCCR1B |= (1<<CS11)|(1<<WGM12); // WGM12 put timer on FASTPWM mode with OCRnA - CS = 010 to put clk at 8
    TCCR1C = 0;
    TIMSK1 = (1<<OCIE1A) ; // To set it with OCR1A
} 

// Initialise le timer 2 en mode CTC avec un prescaler de 1024 
void Timer::timerTwoCTC1024(uint8_t duration)
{
    TCNT2 = 0;
    OCR2A = duration;
    TCCR2A |=(1<<COM2A1)|(1<<WGM21);// Clear OCR2A (OCnA) On compare match WGM21 put timer on CTC mode
    TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); // with OCRnA - CS = 111 to put clk at 1024
    TIMSK2 = (1<<OCIE2A) ; // To set it with OCR2A
}
    
// Initialise le timer 2 en mode CTC avec un prescaler de 8 
void Timer::timerTwoCTC8(uint8_t duration)
{
    TCNT2 = 0;
    OCR2A = duration;
    TCCR2A |=(1<<COM2A1)|(1<<WGM21);// Clear OCR1A (OCnA) On compare match WGM21 put timer on CTC mode
    TCCR2B |= (1<<CS21); // OCRnA - CS = 010 to put clk at 8
    TIMSK2 = (1<<OCIE2A) ; // To set it with OCR2A
}

// Initialise le timer 2 en mode FastPWM avec un prescaler de 1024
void Timer::timerTwoFastPWM1024(uint8_t duration)
{
    TCNT2 = 0;
    OCR2A = duration;
    TCCR2A |=(1<<COM2A1) | (1 << WGM21) | (1 << WGM20);// Clear OCR1A (OCnA) On compare match WGM20 and WGM21 put timer on FASTPWM mode
    TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); // with OCRnA - CS = 111 to put clk at 1024
    TIMSK2 = (1<<OCIE2A) ; // To set it with OCR1A
}

// Initialise le timer 2 en mode FastPWM avec un prescaler de 8
void Timer::timerTwoFastPWM8(uint8_t duration)
{
    TCNT2 = 0;
    OCR2A = duration;
    TCCR2A |=(1<<COM2A1) | (1 << WGM21) | (1 << WGM20);// Clear OCR1A (OCnA) On compare match WGM20 and WGM21 put timer on FASTPWM mode
    TCCR2B |= (1<<CS21); // with OCRnA - CS = 111 to put clk at 8
    TIMSK2 = (1<<OCIE2A) ; // To set it with OCR1A
}
