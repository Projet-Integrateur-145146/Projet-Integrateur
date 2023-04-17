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

int Transmission::orientation(const CustomPair& O, const CustomPair& A, const CustomPair& B)
{
    return (A.first - O.first) * (B.second - O.second) - (A.second - O.second) * (B.first - O.first);
}

float Transmission::distanceSquared(const CustomPair &p1, const CustomPair &p2) {
    return powf(p1.first - p2.first, 2) + powf(p1.second - p2.second, 2);
}

// Comparator function to sort points based on their polar angle with respect to the bottom-most point
bool Transmission::comparePoints(const CustomPair &p1, const CustomPair &p2) {
    int16_t o = orientation(bottomMostPoint, p1, p2);
    if (o == 0) {
        return distanceSquared(bottomMostPoint, p1) < distanceSquared(bottomMostPoint, p2);
    }
    return o > 0;
}

// Swap two CustomPair objects
void Transmission::swap(CustomPair &p1, CustomPair &p2) {
    CustomPair temp = p1;
    p1 = p2;
    p2 = temp;
}

// Bubble sort with custom comparator
void Transmission::sortPoints(CustomPair arr[], uint8_t n) {
    for (uint8_t i = 0; i < n - 1; i++) {
        for (uint8_t j = 0; j < n - i - 1; j++) {
            if (!comparePoints(arr[j], arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void Transmission::convexHull() {
    // Find the bottom-most point
    uint8_t bottomMostIndex = 0;
    for (uint8_t i = 1; i < numberOfPoints; i++) {
        if (arrayOfPairs[i].second < arrayOfPairs[bottomMostIndex].second ||
            (arrayOfPairs[i].second == arrayOfPairs[bottomMostIndex].second &&
             arrayOfPairs[i].first < arrayOfPairs[bottomMostIndex].first)) {
            bottomMostIndex = i;
        }
    }
    bottomMostPoint = arrayOfPairs[bottomMostIndex];

    // Swap the bottom-most point with the first point in the array
    swap(arrayOfPairs[0], arrayOfPairs[bottomMostIndex]);

    // Sort the points based on their polar angle with respect to the bottom-most point
    sortPoints(arrayOfPairs + 1, numberOfPoints - 1);

    // Initialize the hull with the first 3 points
    uint8_t m = 3;
    for (uint8_t i = 3; i < numberOfPoints; i++) {
        // Remove points that form a clockwise turn
        while (m > 1 && orientation(hull[m - 2], hull[m - 1], arrayOfPairs[i]) <= 0) {
            m--;
        }
        hull[m++] = arrayOfPairs[i];
    }

    nElementsHull = m;
}

// Calcule la position sur l'image 
void Transmission::calculatePos(uint8_t index) {
    uint8_t posX = index % 8; 
    uint8_t posY = index / 8;  
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
float Transmission::getAngle(CustomPair point, CustomPair barycenter) {
    CustomPair t;
    t.first = (point.first - barycenter.first); 
    t.second = (point.second - barycenter.second);  

    float angle = (atan2f(t.second, t.first) * 180 / M_PI);
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
    convexHull();
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