/******************************************************************************/
/*                                                                            */
/* Elise Delaplace - 27 février 2023                                          */
/*                                                                            */
/* Cette libraire permet d'utiliser le PWM du microcontroleur ATMega244pa afin*/
/* d'alimenter les roues en utilisant les broches D2 et D3.                   */
/*                                                                            */                                                                         
/******************************************************************************/
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

class PWM{
public:

    PWM(void); 
    ~PWM();

    // ajuster le PWM
    void ajustPWM(double ratio_a,double ratio_b);

    // faire avancer le robot
    void setForwardAll();
    void setForwardLeft();
    void setForwardRight();
    // faire reculer le robot
    void setBackwardAll();
    void setBackwardLeft();
    void setBackwardRight();
    // faire tourner le robot à droite de x degrés
    void setDirection(uint8_t alpha);
    // obtenir le ratio
    uint8_t ratio(uint8_t ratio);

};

#endif
