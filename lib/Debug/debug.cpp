#include "debug.h"

void printDebug(const char *word){
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0x00;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

    int size = strlen(word);
    uint8_t j;

    for (j = 0; j < size ; j++ ) {
        // Attendre que le buffer de transmission soit vide
        while (!(UCSR0A & (1 << UDRE0)));

        // Placer les données dans le buffer de transmission
        UDR0 = word[j];
    }
}
