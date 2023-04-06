#include "transmission.h"

Transmission::Transmission() {
     memoire.initialisationUART();     
     // On fait la lecture de EEPROM et on remplie data 
     memoire.lecture(0, data, 32);
     for (uint8_t i = 0; i < 32; i++) {
        if (data[i] == 1)
            numberOfPoints++;
     }


}

float Transmission::returnValue(CustomPair p1, CustomPair p2, CustomPair p) {
    return  (p.second - p1.second) * (p2.first - p1.first) -
            (p2.second - p1.second) * (p.first - p1.first);
}

// Returns the side of point p with respect to line
// joining points p1 and p2.
float Transmission::findSide(CustomPair p1, CustomPair p2, CustomPair p)
{
    float val = returnValue(p1, p2, p);
    return val > 0 ? 1 : (val < 0 ? -1 : 0);
}
    
// returns a value proportional to the distance
// between the point p and the line joining the
// points p1 and p2
uint16_t Transmission::lineDist(CustomPair p1, CustomPair p2, CustomPair p)
{
    return fabs(returnValue(p1, p2, p));
}
    

bool Transmission::checkIfExist(CustomPair hull[], CustomPair p) {
    for (int i = 0; i < nElementsHull; i++) {
            if (p == hull[i]) {
                return true; 
            }
        }
    return false; 
}

// End points of line L are p1 and p2. side can have value
// 1 or -1 specifying each of the parts made by the line L
void Transmission::quickHull(CustomPair a[], uint8_t n, CustomPair p1, CustomPair p2, int8_t side)
{
    int8_t ind = -1;
    float max_dist = 0;
    
    // finding the point with maximum distance
    // from L and also on the specified side of L.
    for (uint8_t i=0; i<n; i++)
    {
        uint16_t temp = lineDist(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }
    
    // If no point is found, add the end points
    // of L to the convex hull.
    if (ind == -1)
    {
        if (!checkIfExist(hull, p1)) 
            hull[nElementsHull++] = p1; 
        if (!checkIfExist(hull, p2))
            hull[nElementsHull++] = p2;
        return;
    }
    
    // Recur for the two parts divided by a[ind]
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
}
    
void Transmission::printHull(CustomPair a[], uint8_t n)
{    
    // Finding the point with minimum and
    // maximum x-coordinate
    uint8_t min_x = 0, max_x = 0;
    for (uint8_t i=1; i<n; i++)
    {
        if (a[i].first < a[min_x].first)
            min_x = i;
        if (a[i].first > a[max_x].first)
            max_x = i;
    }
    
    // Recursively find convex hull points on
    // one side of line joining a[min_x] and
    // a[max_x]
    quickHull(a, n, a[min_x], a[max_x], 1);
    
    // Recursively find convex hull points on
    // other side of line joining a[min_x] and
    // a[max_x]
    quickHull(a, n, a[min_x], a[max_x], -1);
}


void Transmission::calculatePos(uint8_t index) {
    float posX = index % 8; 
    float posY = index / 8;  
    CustomPair t;
    t.first = xInit + (GAP_CIRCLES_X * (posX)); 
    t.second = HEIGHT - (yInit + (GAP_CIRCLES_Y * (posY))); 
    arrayOfPairs[compteurPair++] = t; 
}

void Transmission::findPos() {
    for (uint8_t index = 0; index < SIZE_DATA; index++) {
        if (data[index] == 1) {
            calculatePos(index);
        }
    }
}

// Va nous permettre de trouver le barycentre de tout les points 
CustomPair Transmission::getBarycenter() {
    float sumX = 0;
    float sumY = 0; 

    for (uint8_t i = 0; i <= nElementsHull; i++) {
        sumX += hull[i].first; 
        sumY += hull[i].second; 
    }
        
    CustomPair barycentre; 
    barycentre.first = sumX/nElementsHull; 
    barycentre.second = sumY/nElementsHull; 
    return barycentre; 
}

// Permet d'avoir l'angle d'un point par rapport au barycentre
float Transmission::getAngle(CustomPair point, CustomPair barycenter) {
    CustomPair t;
    t.first = (point.first - barycenter.first); 
    t.second = (point.second - barycenter.second);  

    float angle = (atan2f(t.second, t.first) * 180 / M_PI);
    angle+=(angle<0)*360 ;
    return angle;
}    

void Transmission::sortList() {
    CustomPair barycenter = getBarycenter();
    for (uint8_t i = 0; i < nElementsHull; i++) {
        for (uint8_t j = i; j < nElementsHull; j++) {
            if (getAngle(hull[i], barycenter) > getAngle(hull[j], barycenter)) {
                CustomPair tempPoint = hull[i]; 
                hull[i] = hull[j]; 
                hull[j] = tempPoint;
                break;  
            }
        }
    }
}

void Transmission::fillLines() {
    sortList(); 
    transmissionTableau("<polygon points=\""); 
    for (uint8_t i = 0; i < nElementsHull; i++) {
        transmettreFloat(hull[i].first); 
        transmissionTableau(","); 
        transmettreFloat(hull[i].second);
        transmissionTableau(" ");
    }
    transmissionTableau("\" stroke=\"black\" stroke-width=\"4\" fill=\"green\"/>");
}

void Transmission::generateLines() {
    fillLines(); 
}

// Generates circles on the board if they are detected by the robot
void Transmission::generateCircles() {
    for (uint8_t index = 0; index < numberOfPoints; index++) {
        CustomPair t = arrayOfPairs[index];
        transmissionTableau("<circle cx=\""); 
        transmettreFloat(t.first);
        transmissionTableau("\" cy=\"");
        transmettreFloat(t.second);
        transmissionTableau("\" style=\"z-index:1\" stroke=\"black\" stroke-width=\"2\" r=\"10\" fill=\"grey\"/>"); 
    }
}

// Generates squares on the board
void Transmission::generateSquares() {
    float posY = yInit; 
    for (uint8_t i = 0; i < 4; i++) {
        float posX = xInit;
        for (uint8_t j = 0; j < 8; j++){
            transmissionTableau("<rect x=\""); 
            transmettreFloat(posX);
            transmissionTableau("\" y=\"");  
            transmettreFloat(posY);
            transmissionTableau("\" width=\"5\" height=\"5\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"); 
            posX += 110;
        }
        posY += 110; 
    }
}

// Generates the header of the svg file 
void Transmission::generateHeader() {
    transmissionTableau("<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1152 576\"> <rect x=\"96\" y=\"48\" width=\"960\" height=\"480\" stroke=\"black\" stroke-width=\"1\" fill=\"white\"/>"); 
}

float Transmission::getAire() {
    float S1 = 0, S2 = 0;

    // Calculate S1 and S2
    for (uint8_t i = 0; i < nElementsHull; i++) {
        uint8_t j = (i + 1) % nElementsHull;
        S1 += hull[i].first * hull[j].second;
        S2 += hull[i].second * hull[j].first;
    }

    // Aire
    float airePixel = 0.5 * fabs(S1 - S2);
    float pixel_size_x = 1 / 10.0; 
    float pixel_size_y = 1 / 10.0; 
    return airePixel * pixel_size_x * pixel_size_y; 
} 

// Generates the end of the svg file
void Transmission::generateEnd() {
    float aire = getAire(); 
    transmissionTableau("<text x=\"96\" y=\"36\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">section 06 -- equipe 145146 -- VROUMY</text>");     
    transmissionTableau("<text x=\"96\" y=\"552\" font-family=\"arial\" font-size=\"20\" fill=\"blue\"> AIRE: "); 
    transmettreFloat(aire);
    transmissionTableau(" pouces carres</text> </svg>"); 
}

void Transmission::transmettreFloat(float value) {
    // Cast float to integer and extract digits
    uint16_t integerPart = (uint16_t)value;
    uint16_t decimalPart = fabs((uint16_t)((value - integerPart) * 100));

    // Send integer part digits to UART
    for (uint16_t i = 1000; i > 0; i /= 10) {
        transmissionAndUpdtateCRC('0' + ((integerPart / i) % 10));
    }

    // Send decimal point separator
    transmissionAndUpdtateCRC('.');

    // Send decimal part digits to UART
    for (uint8_t i = 10; i > 0; i /= 10) {
        transmissionAndUpdtateCRC('0' + ((decimalPart / i) % 10));
    }
}

void Transmission::transmissionTableau(const char* dataSVG) {
    for (uint8_t i = 0; dataSVG[i] != '\0'; i++) {
        transmissionAndUpdtateCRC(dataSVG[i]);
    }
}

void Transmission::transmettreUint32(uint32_t value) {
    const char hexDigits[] = "0123456789abcdef";

    for (int i = 0; i < 8; ++i) {
        uint8_t hexDigitIndex = (value >> (4 * (7 - i))) & 0xF;
        char hexDigit = hexDigits[hexDigitIndex];
        memoire.transmissionUART(hexDigit);
    }
}

void Transmission::transmissionAndUpdtateCRC(uint8_t charactere) {
    if (fini == false) {
        updateCRC(charactere);   
        memoire.transmissionUART(charactere); 
    }
}

//-------------------------Calcul CRC 32--------------------------------
// Code tiré du site: https://www.carnetdumaker.net/articles/les-sommes-de-controle/


void Transmission::updateCRC(uint8_t data_byte) {
    crc ^= static_cast<uint32_t>(data_byte);
    for (int i = 0; i < 8; ++i) {
        if (crc & 1) {
            crc = (crc >> 1) ^ 0xEDB88320;
        } else {
            crc = crc >> 1;
        }
    }   
}

void Transmission::generateSVG() {
    //--------------------- NEW CPP ------------------------
    uint8_t debut = 0x02;
    memoire.transmissionUART(debut); 
    findPos(); 
    generateHeader(); 
    printHull(arrayOfPairs, numberOfPoints);
    generateLines(); 
    generateSquares(); 
    generateCircles();
    generateEnd();
    fini = true;
    uint8_t fin = 0x03; 
    memoire.transmissionUART(fin); 
    crc = crc ^ 0xFFFFFFFF;
    transmettreUint32(crc); 
    fin = 0x04; 
    memoire.transmissionUART(fin); 
}
