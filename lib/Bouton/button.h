#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#define BUTTON_DEBOUNCE_DELAY 15
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Utiliser dans les autre fonction set. Sert a enable les interruptions de type INTX
void setButtonInterruption(uint8_t = INT0);

// Les trois fonctions suivantes servent a set les parametres des interruptions lors de l'appuie et/ou de la relache du bouton
void setButtonAnyEdge(volatile uint8_t* registers = &DDRD, uint8_t pin = PIND2, uint8_t INTX = INT0);

void setButtonFallingEdge(volatile uint8_t* registers = &DDRD, uint8_t pin = PIND2, uint8_t INTX = INT0);

void setButtonRisingEdge(volatile uint8_t* registers = &DDRD, uint8_t pin = PIND2, uint8_t INTX = INT0);

// Test si le bouton est appuye
bool isButtonPressed(volatile uint8_t* registers = &PIND, uint8_t pin = PIND2);