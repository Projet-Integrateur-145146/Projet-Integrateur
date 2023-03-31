#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>

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
    Transmission() {
        // Valeurs pour tester 

        data[9] = 1; 
        data[17] = 1;
        data[25] = 1;
        data[27] = 1; 
        data[18] = 1; 
        data[20] = 1; 

        /*
        data[0] = 1; 
        data[1] = 1; 
        data[8] = 1; 
        data[9] = 1; 
        data[16] = 1; 
        data[17] = 1;       
        */

        /*
        data[2] = 1; 
        data[1] = 1; 
        data[10] = 1; 
        data[9] = 1; 
        */
    }; 
    ~Transmission() { delete[] data; }

    int returnValue(CustomPair p1, CustomPair p2, CustomPair p) {
        return  (p.second - p1.second) * (p2.first - p1.first) -
                (p2.second - p1.second) * (p.first - p1.first);
    }

    // Returns the side of point p with respect to line
    // joining points p1 and p2.
    float findSide(CustomPair p1, CustomPair p2, CustomPair p)
    {
        float val = returnValue(p1, p2, p);
        return val > 0 ? 1 : (val < 0 ? -1 : 0);
    }
    
    // returns a value proportional to the distance
    // between the point p and the line joining the
    // points p1 and p2
    int lineDist(CustomPair p1, CustomPair p2, CustomPair p)
    {
        return abs(returnValue(p1, p2, p));
    }
    

    bool checkIfExist(CustomPair hull[], CustomPair p) {
        for (int i = 0; i < nElementsHull; i++) {
                if (p == hull[i]) 
                    return true; 
            }
        return false; 
    }

    // End points of line L are p1 and p2. side can have value
    // 1 or -1 specifying each of the parts made by the line L
    void quickHull(CustomPair a[], int n, CustomPair p1, CustomPair p2, int side)
    {
        int ind = -1;
        float max_dist = 0;
    
        // finding the point with maximum distance
        // from L and also on the specified side of L.
        for (int i=0; i<n; i++)
        {
            int temp = lineDist(p1, p2, a[i]);
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
    
    void printHull(CustomPair a[], int n)
    {    
        // Finding the point with minimum and
        // maximum x-coordinate
        int min_x = 0, max_x = 0;
        for (int i=1; i<n; i++)
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

    CustomPair calculatePos(int index) {
        int posX = index % 8; 
        int posY = index / 8;  
        CustomPair t;
        t.first = xInit + (GAP_CIRCLES_X * (posX)); 
        t.second = HEIGHT - (yInit + (GAP_CIRCLES_Y * (posY))); 
        arrayOfPairs[compteurPair++] = t; 
        return t; 
    }

    // Va nous permettre de trouver le barycentre de tout les points 
    CustomPair getBarycenter() {
        float sumX = 0;
        float sumY = 0; 

        for (int i = 0; i <= nElementsHull; i++) {
            sumX += hull[i].first; 
            sumY += hull[i].second; 
        }
        
        CustomPair barycentre; 
        barycentre.first = sumX/nElementsHull; 
        barycentre.second = sumY/nElementsHull; 
        return barycentre; 
    }

    // Permet d'avoir l'angle d'un point par rapport au barycentre
    float getAngle(CustomPair point, CustomPair barycenter) {
        CustomPair t;
        t.first = (point.first - barycenter.first); 
        t.second = (point.second - barycenter.second);  

        float angle = (atan2f(t.second, t.first) * 180 / M_PI);
        angle+=(angle<0)*360 ;
        return angle;
    }    

    void sortList() {
        CustomPair barycenter = getBarycenter();
        for (int i = 0; i < nElementsHull; i++) {
            for (int j = i; j < nElementsHull; j++) {
                if (getAngle(hull[i], barycenter) > getAngle(hull[j], barycenter)) {
                    CustomPair tempPoint = hull[i]; 
                    hull[i] = hull[j]; 
                    hull[j] = tempPoint;
                    break;  
                }
            }
        }
    }

    std::string fillLines() {
        sortList(); 
        std::string svgData = "<polygon points=\""; 
        for (int i = 0; i < nElementsHull; i++) {
            svgData += std::to_string(hull[i].first) + "," + std::to_string(hull[i].second) + " "; // "308,144 934,144 308,336";
        }
        svgData += "\" stroke=\"black\" stroke-width=\"4\" fill=\"green\"/>";
        return svgData; 
    }

    std::string generateLines() {
        return fillLines(); 
    }

    // Generates circles on the board if they are detected by the robot
    std::string generateCircles() {
        std::string svgData = ""; 

        for (int index = 0; index < SIZE_DATA; index++) {
            if (data[index] == 1) {
                CustomPair t = calculatePos(index);
                svgData += "<circle cx=\"" + std::to_string(t.first) + "\" cy=\"" + std::to_string(t.second) +  "\" style=\"z-index:1\" stroke=\"black\" stroke-width=\"2\" r=\"10\" fill=\"grey\"/>";
            }
        }
        return svgData;
    }

    // Generates squares on the board
    std::string generateSquares() {
        std::string svgData = ""; 
        int posY = yInit; 
        for (int i = 0; i < 4; i++) {
            int posX = xInit;
            for (int j = 0; j < 8; j++){
                svgData += "<rect x=\"" + std::to_string(posX) + "\" y=\"" + std::to_string(posY) + "\" width=\"5\" height=\"5\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>";
                posX += 110;
            }
            posY += 110; 
        }
        return svgData;
    }

    // Generates the header of the svg file 
    std::string generateHeader() {
        std::string head = "<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1152 576\"> <rect x=\"96\" y=\"48\" width=\"960\" height=\"480\" stroke=\"black\" stroke-width=\"1\" fill=\"white\"/>";
        return head; 
    }

    int getAire() {
        float S1 = 0, S2 = 0;

        // Calculate S1 and S2
        for (int i = 0; i < nElementsHull; i++) {
            int j = (i + 1) % nElementsHull;
            S1 += hull[i].first * hull[j].second;
            S2 += hull[i].second * hull[j].first;
        }

        // Aire
        float airePixel = 0.5 * abs(S1 - S2);
        float pixel_size_x = 1 / 10.0; 
        float pixel_size_y = 1 / 10.0; 
        return airePixel * pixel_size_x * pixel_size_y; 
    } 

    // Generates the end of the svg file
    std::string generateEnd() {
        int aire = getAire(); 
        std::string end = "<text x=\"96\" y=\"36\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">section 06 -- équipe 145146 -- VROUMY</text>";     
        end += "<text x=\"96\" y=\"552\" font-family=\"arial\" font-size=\"20\" fill=\"blue\"> AIRE: " + std::to_string(aire) + " pouces carrés</text> "; 
        end += "</svg>";
        return end; 
    }

    void generateSVG() {
        std::ofstream fichier("test.svg");
        fichier << generateHeader();
        std::string data = generateCircles(); 
        printHull(arrayOfPairs, numberOfPoints);
        fichier << generateLines(); 
        fichier << generateSquares();
        fichier << data; 
        fichier << generateEnd(); 
        fichier.close();
    }

private: 
    int* data = new int[SIZE_DATA];
    int numberOfPoints = 6;  
    CustomPair arrayOfPairs[6]; 
    int compteurPair = 0;
    CustomPair hull[10];
    int nElementsHull = 0; 
    float xInit = 191;
    float yInit = 123;
};