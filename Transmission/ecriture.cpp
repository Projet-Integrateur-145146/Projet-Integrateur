#define F_CPU 8000000UL
#include <avr/io.h> 
#include <RS232/memoire_24.h>

int main() {
    Memoire24CXXX memoire;
    memoire.initialisationUART();     
    // On ecrit dans la memoire
    uint8_t motEcrit[32] = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0};
    memoire.ecriture(0, motEcrit, 32);
}