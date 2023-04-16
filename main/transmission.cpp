#include "transmission.h"

Transmission::Transmission() {
    memoire.initialisationUART();    

/*
    uint8_t motEcrit[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    motEcrit[11] = 1; 
    motEcrit[13] = 1; 
    motEcrit[18] = 1; 
   motEcrit[19] = 1;
    motEcrit[29] = 1; 
    memoire.ecriture(0, motEcrit, 32);    
*/
    // On fait la lecture de EEPROM et on remplie data 
    /*
    memoire.lecture(0, data, 32);
    for (uint8_t i = 0; i < 32; i++) {
       if (data[i] == 1)
           numberOfPoints++;
    }
    */
}

int16_t Transmission::crossProduct(CustomPair &O, CustomPair &A, CustomPair &B) {
    return (A.first - O.first) * (B.second - O.second) - (A.second - O.second) * (B.first - O.first);
}

void Transmission::swapCustomPair(CustomPair &a, CustomPair &b) {
    CustomPair temp = a;
    a = b;
    b = temp;
}

int32_t Transmission::squaredDistance(CustomPair &a, CustomPair &b) {
    int16_t dx = a.first - b.first;
    int16_t dy = a.second - b.second;
    return dx * dx + dy * dy;
}

bool Transmission::comparePolarAngles(CustomPair &a, CustomPair &b, CustomPair &p0) {
    int16_t cp = crossProduct(p0, a, b);
    if (cp == 0) {
        return squaredDistance(p0, a) <= squaredDistance(p0, b);
    }
    return cp > 0;
}

void Transmission::grahamScan(CustomPair points[], uint8_t n, CustomPair hull[], uint8_t &hullSize) {
    // Find the lowest point
    uint8_t minYIndex = 0;
    for (uint8_t i = 1; i < n; i++) {
        if (points[i].second < points[minYIndex].second || (points[i].second == points[minYIndex].second && points[i].first < points[minYIndex].first)) {
            minYIndex = i;
        }
    }

    // Swap the lowest point with the first element
    swapCustomPair(points[0], points[minYIndex]);

    // Sort the remaining points based on their polar angles with respect to the lowest point
    CustomPair p0 = points[0];
    for (uint8_t i = 1; i < n - 1; i++) {
        uint8_t minIndex = i;
        for (uint8_t j = i + 1; j < n; j++) {
            if (comparePolarAngles(points[j], points[minIndex], p0)) {
                minIndex = j;
            }
        }
        swapCustomPair(points[i], points[minIndex]);
    }

    // Initialize the convex hull with the first three points
    hullSize = 0;
    hull[hullSize++] = points[0];
    hull[hullSize++] = points[1];
    hull[hullSize++] = points[2];

    // Process the remaining points
    for (uint8_t i = 3; i < n; i++) {
        // Remove points from the hull while they make a non-left turn
        while (hullSize > 1 && crossProduct(hull[hullSize - 2], hull[hullSize - 1], points[i]) <= 0) {
            hullSize--;
        }
        hull[hullSize++] = points[i];
    }
}

// Calcule la position sur l'image 
void Transmission::calculatePos(uint8_t index) {
    int16_t posX = index % 8; 
    int16_t posY = index / 8;  
    CustomPair t;
    t.first = xInit + (GAP_CIRCLES_X * (posX)); 
    t.second = HEIGHT - (yInit + (GAP_CIRCLES_X * (posY))); 
    arrayOfPairs[compteurPair++] = t; 
}

void Transmission::findPos() {
    for (uint8_t index = 0; index < SIZE_DATA; index++) {
        if (data[index] == 1) {
            calculatePos(index);
        }
    }
}

// Permet de trouver le barycentre de tout les points
CustomPair Transmission::getBarycenter() {
    uint16_t sumX = 0;
    uint16_t sumY = 0; 

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
int16_t Transmission::getAngle(CustomPair point, CustomPair barycenter) {
    CustomPair t;
    t.first = (point.first - barycenter.first); 
    t.second = (point.second - barycenter.second);  

    int16_t angle = (atan2f(t.second, t.first) * 180 / M_PI);
    angle+=(angle<0)*360 ;
    return angle;
}    

// Mettre en ordre la liste de point formant la forme convexe
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

// Permet de remplir la forme dans le SVG
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

// Dessine des cercles sur le SVG
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

// Dessine des carrés sur le SVG
void Transmission::generateSquares() {
    int16_t posY = yInit; 
    for (uint8_t i = 0; i < 4; i++) {
        int16_t posX = xInit;
        for (uint8_t j = 0; j < 8; j++){
            transmissionTableau("<rect x=\""); 
            transmettreFloat(posX);
            transmissionTableau("\" y=\"");  
            transmettreFloat(posY);
            transmissionTableau("\" width=\"5\" height=\"5\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"); 
            posX += GAP_CIRCLES_X;
        }
        posY += GAP_CIRCLES_X; 
    }
}

// Genere le header du fichier SVG
void Transmission::generateHeader() {
    transmissionTableau("<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1152 576\"> <rect x=\"96\" y=\"48\" width=\"960\" height=\"480\" stroke=\"black\" stroke-width=\"1\" fill=\"white\"/>"); 
}

uint16_t Transmission::getAire() {
    float S1 = 0, S2 = 0;

    // Calculate S1 and S2
    for (uint8_t i = 0; i < nElementsHull; i++) {
        uint8_t j = (i + 1) % nElementsHull;
        S1 += hull[i].first * hull[j].second;
        S2 += hull[i].second * hull[j].first;
    }
    return 0.5 * fabs(S1 - S2) * 0.1 * 0.1; 
} 

// Genere la fin du SVG
void Transmission::generateEnd() {
    float aire = getAire(); 
    transmissionTableau("<text x=\"96\" y=\"36\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">section 06 -- equipe 145146 -- VROUMY</text>");     
    transmissionTableau("<text x=\"96\" y=\"552\" font-family=\"arial\" font-size=\"20\" fill=\"blue\"> AIRE: "); 
    transmettreFloat(aire);
    transmissionTableau(" pouces carres</text> </svg>"); 
}

void Transmission::transmettreFloat(uint16_t value) {
    for (uint32_t i = 10000; i > 0; i /= 10) {
        transmissionAndUpdtateCRC('0' + ((value / i) % 10));
    }
}

// Transmettre un tableau de donnés au UART
void Transmission::transmissionTableau(const char* dataSVG) {
    for (uint8_t i = 0; dataSVG[i] != '\0'; i++) {
        transmissionAndUpdtateCRC(dataSVG[i]);
    }
}

// Transmettre un integer au UINT
void Transmission::transmettreUint32(uint32_t value) {
    uint8_t hexDigits[] = "0123456789abcdef";

    for (int i = 0; i < 8; ++i) {
        uint8_t hexDigitIndex = (value >> (4 * (7 - i))) & 0xf;
        uint8_t hexDigit = hexDigits[hexDigitIndex];
        memoire.transmissionUART(hexDigit);
    }
}

// Transmettre une valeur au UART et update la valeur du CRC
void Transmission::transmissionAndUpdtateCRC(uint8_t charactere) {
    if (fini == false) {
        updateCRC(charactere);   
        memoire.transmissionUART(charactere); 
    }
}

//-------------------------Calcul CRC 32--------------------------------
// Code tirÃ© du site: https://www.carnetdumaker.net/articles/les-sommes-de-controle/

void Transmission::updateCRC(uint8_t data_byte) {
    crc ^= static_cast<uint32_t>(data_byte);
    for (int i = 0; i < NB_BYTES_CRC; ++i) {
        if (crc & 1) {
            crc = (crc >> BITSHIFT) ^ POLYNOME;
        } else {
            crc = crc >> BITSHIFT;
        }
    }   
}

void Transmission::generateSVG() {
    //--------------------- NEW CPP ------------------------
    memoire.transmissionUART(DEBUT); 
    findPos(); 
    generateHeader(); 
    //printHull(arrayOfPairs, numberOfPoints);
    grahamScan(arrayOfPairs, numberOfPoints, hull, nElementsHull);
    generateLines(); 
    generateSquares(); 
    generateCircles();
    generateEnd();
    fini = true;
    memoire.transmissionUART(TRANSMISSION_CRC); 
    crc = crc ^ CRC_START_VALUE;
    transmettreUint32(crc); 
    memoire.transmissionUART(FIN); 
}