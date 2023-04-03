/******************************************************************************/
/*                                                                            */
/* Ce code a ete ecrit par Elise DELAPLACE pour le cours de INF1900           */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                               PWM.cpp                                      */
/*                                                                            */
/*  Cette libraire permet d'utiliser le PWM du microcontroleur ATMega244pa    */
/* afin d'alimenter les roues en utilisant les broches D3(D) et D5(E-OCR1A)   */
/* pour la roue de gauche et D4(D) et D6(E-OCR1B) pour la roue de droite.     */   
/*                                                                            */
/******************************************************************************/


#include "pwm.h"

  
/******************************************************************************/
/* void PWM::ajustPWM(uint8_t rapport)                                        */
/*                                                                            */
/*      Constructeur: ajuste le pwm à fournir                                 */ 
/*                                                                            */
/* Parametre d'entree  : uint8_t rapport - rapport auquel le pwm tourne       */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void PWM::ajustPWM(float ratio_a, float ratio_b){
    TCNT1 = 0x0;
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    OCR1A = (0xFF * (ratio_a / 100.0));
    OCR1B = (0xFF * (ratio_b / 100.0));
    //0xFF * (rapport / 100)
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0);
    TCCR1B |= (1 << CS11); //|w(1<<WGM10)|(1<<WGM11)|(1<<CS11);(1<<WGM13)|
    TCCR1C = 0;
}

/******************************************************************************/
/* void PWM::~PWM()                                                           */
/*                                                                            */
/*      Destructeur: ne fait rien                                             */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
PWM::~PWM(){}

/******************************************************************************/
/* void PWM::PWM()                                                            */
/*                                                                            */
/*      Destructeur: output les ports D                                       */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
PWM::PWM(){
    DDRD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
}

/******************************************************************************/
/* void PWM::setForward(void)                                                 */
/*                                                                            */
/*      Permet en robot d'avancer en avant                                    */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void PWM::setForwardRight(){
    PORTD &= ~(1 << PORTD3);   
}
void PWM::setForwardLeft(){
    PORTD &= ~(1 << PORTD2);  
}
void PWM::setForwardAll(){
    setForwardRight();
    setForwardLeft(); 
}

/******************************************************************************/
/* void PWM::setBackward(void)                                                 */
/*                                                                            */
/*      Permet en robot d'avancer en arrière                                  */
/*                                                                            */
/* Parametre d'entree  : aucun                                                */
/* Parametre de sortie : aucun                                                */
/******************************************************************************/
void PWM::setBackwardRight(){
    PORTD |= (1<<PORTD3);
}
void PWM::setBackwardLeft(){
    PORTD |= (1<<PORTD2);
}
void PWM::setBackwardAll(){
    setBackwardRight();
    setBackwardLeft();
}

/*******************************************************************************/
/* void PWM:: setTurning(uint8_t alpha)                                        */
/*                                                                             */
/*      Permet en robot de tourner d'un angle compris entre -180 et 180 degres.*/                
/*                                                                             */
/* Parametre d'entree  : uint8_t alpha                                         */
/* Parametre de sortie : uint8_t ratio de a et b a fournir au PWM              */
/*******************************************************************************/
void PWM::setDirection(uint8_t alpha){
    float a = 0;
    float b = 0;
    if((alpha > 0 ) && (alpha != 180) && (alpha != -180)){
        b =  100;
        a = (45 * b)/alpha;
    }
    else if((alpha < 0 ) && (alpha != 180) && (alpha != -180)){
        a =  100;
        b = (45 * a)/alpha;
    }
    else if(alpha == 180){
        b =  100;
        a = 0;
    }
    else if(alpha == -180){
        b =  0;
        a = 100;
    }
    else if(alpha==0){
        b = 100;
        a = 100; 
    }
    ajustPWM(ratio(a),ratio(b));
}


/******************************************************************************/
/* uint8_t PWM::ratio(uint8_t rapport)                                        */
/*                                                                            */
/*      Obtenir le ratio à partir du rapport                                  */
/*                                                                            */
/* Parametre d'entree  : uint8_t rapport - le rapport à fournir               */
/* Parametre de sortie : uint8_t      - la ratio obtenu                       */
/******************************************************************************/
uint8_t PWM::ratio(uint8_t ratio){
    ratio = 100 - ratio;
    return ratio;
}

