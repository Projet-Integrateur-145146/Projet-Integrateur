#ifndef COMMON_H
#define COMMON_H
#include "../common.hpp"
#endif
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define F_CPU 8000000UL
#define M_PI 3.14159265358979323846
#define SIZE_DATA 32

const uint8_t GAP_CIRCLES_X = 110;
const uint16_t HEIGHT = 576;
const uint32_t POLYNOME = 0xEDB88320;
const uint32_t CRC_START_VALUE = 0xFFFFFFFF;
const uint8_t DEBUT = 0x02;
const uint8_t TRANSMISSION_CRC = 0x03;
const uint8_t FIN = 0x04;
const uint8_t NB_BYTES_CRC = 8;
const uint8_t BITSHIFT = 1;

struct CustomPair {
    int32_t first;
    int32_t second;

    friend bool operator==(const CustomPair a, const CustomPair b) {
        return a.first == b.first && a.second == b.second;
    }

    friend bool operator<(const CustomPair a, const CustomPair b) {
        return a.first < b.first || a.second < b.second; 
    }
}; 



class Transmission
{
public: 
    Transmission();

    uint8_t orientation(const CustomPair &pointQ, const CustomPair &pointR, const CustomPair &pointS);
    void JARVIS();

    void calculatePos(uint8_t index);
    void findPos();

    // Va nous permettre de trouver le barycentre de tout les points 
    CustomPair getBarycenter();

    // Permet d'avoir l'angle d'un point par rapport au barycentre
    float getAngle(CustomPair point, CustomPair barycenter); 

    void sortList();

    void fillLines();

    void generateLines();

    // Generates circles on the board if they are detected by the robot
    void generateCircles();

    // Generates squares on the board
    void generateSquares();

    // Generates the header of the svg file 
    void generateHeader();

    uint16_t getAire();
    // Generates the end of the svg file
    void generateEnd();
    void transmettreFloat(uint16_t value);

    void transmissionTableau(const char* dataSVG);

    void transmissionAndUpdtateCRC(uint8_t);  

    void transmettreUint32(uint32_t value);

    void updateCRC(uint8_t data_byte);

    void generateSVG();

private: 
    Memoire24CXXX memoire; 
    uint8_t data[SIZE_DATA]; //fait
    uint8_t numberOfPoints = 0;  //fait
    CustomPair arrayOfPairs[10]; 
    uint8_t compteurPair = 0;
    CustomPair hull[10];
    uint8_t nElementsHull = 0; 
    uint16_t xInit = 191;
    uint16_t yInit = 123;
    bool fini = false;
    CustomPair bottomMostPoint;
    uint32_t crc = 0xFFFFFFFF;
};