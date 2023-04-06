//#include <iostream>
//#include <fstream>
#include <avr/io.h> 
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <RS232/memoire_24.h>

#define F_CPU 8000000UL
#define SIZE_DATA 32
#define GAP_CIRCLES_X 110
#define GAP_CIRCLES_Y 110
#define HEIGHT 576 
#define M_PI   3.14159265358979323846 /* pi */

struct CustomPair {
    float first;
    float second;

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

    float returnValue(CustomPair p1, CustomPair p2, CustomPair p);

    // Returns the side of point p with respect to line
    // joining points p1 and p2.
    float findSide(CustomPair p1, CustomPair p2, CustomPair p);
        
    // returns a value proportional to the distance
    // between the point p and the line joining the
    // points p1 and p2
    uint16_t lineDist(CustomPair p1, CustomPair p2, CustomPair p);

    bool checkIfExist(CustomPair hull[], CustomPair p);

    // End points of line L are p1 and p2. side can have value
    // 1 or -1 specifying each of the parts made by the line L
    void quickHull(CustomPair a[], uint8_t n, CustomPair p1, CustomPair p2, int8_t side);
        
    void printHull(CustomPair a[], uint8_t n);

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

    float getAire();

    // Generates the end of the svg file
    void generateEnd();

    void transmettreFloat(float value);

    void transmissionTableau(const char* dataSVG);

    void transmissionAndUpdtateCRC(uint8_t);  

    void transmettreUint32(uint32_t value);

    void updateCRC(uint8_t data_byte);

    void generateSVG();

private: 
    Memoire24CXXX memoire; 
    uint8_t data[SIZE_DATA]; //fait
    uint8_t numberOfPoints = 0;  //fait
    CustomPair arrayOfPairs[13]; 
    uint8_t compteurPair = 0;
    CustomPair hull[13];
    uint8_t nElementsHull = 0; 
    float xInit = 191;
    float yInit = 123;
    uint32_t compteurData = 0; 
    bool fini = false;
    uint32_t crc = 0xFFFFFFFF;
};