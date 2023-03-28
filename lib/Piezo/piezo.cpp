#include "piezo.h"

void Piezo::playSound(uint8_t note) {
switch (note)
    {
    case 45:
        frequency = 110;
        break;
    case 46:
        frequency = 116.54;
        break;
    case 47:
        frequency = 123.47;
        break;
    case 48:
        frequency = 130.81;
        break;
    case 49:
        frequency = 138.59;
        break;
    case 50:
        frequency = 146.83;
        break;
    case 51:
        frequency = 155.56;
        break;
    case 52:
        frequency = 164.81;
        break;
    case 53:
        frequency = 174.61;
        break;
    case 54:
        frequency = 185;
        break;
    case 55:
        frequency = 196;
        break;
    case 56:
        frequency = 207.65;
        break;
    case 57:
        frequency = 220;
        break;
    case 58:
        frequency = 233.08;
        break;
    case 59:
        frequency = 246.94;
        break;
    case 60:
        frequency = 261.63;
        break;
    case 61:
        frequency = 277.18;
        break;
    case 62:
        frequency = 293.66;
        break;
    case 63:
        frequency = 311.13;
        break;
    case 64:
        frequency = 329.63;
        break;
    case 65:
        frequency = 349.23;
        break;
    case 66:
        frequency = 369.99;
        break;
    case 67:
        frequency = 392;
        break;
    case 68:
        frequency = 415.30;
        break;
    case 69:
        frequency = 440;
        break;
    case 70:
        frequency = 466.16;
        break;
    case 71:
        frequency = 493.88;
        break;
    case 72:
        frequency = 523.25;
        break;
    case 73:
        frequency = 554.37;
        break;
    case 74:
        frequency = 587.33;
        break;
    case 75:
        frequency = 622.25;
        break;
    case 76:
        frequency = 659.26;
        break;
    case 77:
        frequency = 698.46;
        break;
    case 78:
        frequency = 739.99;
        break;
    case 79:
        frequency = 783.99;
        break;
    case 80:
        frequency = 830.61;
        break;
    case 81:
        frequency = 880;
        break;
    }

   TCNT2 = 0;
   TCCR2A |= (1 << COM2A0);
   OCR2A = (8000000.0/(2.0*256.0*(frequency)))-1;
 }

 void Piezo::stopSound() {
   TCCR2A &= ~(1<<COM2A0);
 }