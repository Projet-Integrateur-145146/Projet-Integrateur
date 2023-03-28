#ifndef DEBUG_H
#define DEBUG_H

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

//#define DEBUG

#ifdef DEBUG

#define debug_printf(x) printDebug(x) // debug_printf = printDebug

#else // Si DEBUG n'est pas défini

#define debug_printf(x) do {} while(0)// Ne fait rien

#endif

void printDebug(const char *word);

#endif