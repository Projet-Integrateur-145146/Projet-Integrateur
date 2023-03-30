#include <iostream>
#include <fstream>
#include <set>

#define SIZE_DATA 32
#define GAP_CIRCLES_X 106
#define GAP_CIRCLES_Y 96
#define HEIGHT 576 

class Transmission
{
public: 
    Transmission() {
        // Valeurs pour tester 
        data[0] = 1;
        data[1] = 1; 
        data[8] = 1;  

        // Il faudra lire les données au début 
        //std::pair<int, int>* arrayOfPairs = new std::pair<int, int>[3]; 
    }; 
    ~Transmission() { delete[] data; }

    // Returns the side of point p with respect to line
    // joining points p1 and p2.
    int findSide(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p)
    {
        int val = (p.second - p1.second) * (p2.first - p1.first) -
                (p2.second - p1.second) * (p.first - p1.first);
    
        if (val > 0)
            return 1;
        if (val < 0)
            return -1;
        return 0;
    }
    
    // returns a value proportional to the distance
    // between the point p and the line joining the
    // points p1 and p2
    int lineDist(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p)
    {
        return abs ((p.second - p1.second) * (p2.first - p1.first) -
                (p2.second - p1.second) * (p.first - p1.first));
    }
    
    // End points of line L are p1 and p2. side can have value
    // 1 or -1 specifying each of the parts made by the line L
    void quickHull(std::pair<int, int> a[], int n, std::pair<int, int> p1, std::pair<int, int> p2, int side)
    {
        int ind = -1;
        int max_dist = 0;
    
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
            hull.insert(p1);
            hull.insert(p2);
            return;
        }
    
        // Recur for the two parts divided by a[ind]
        quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
        quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
    }
    
    void printHull(std::pair<int, int> a[], int n)
    {
        // a[i].second -> y-coordinate of the ith point
        if (n < 3)
        {
            std::cout << "Convex hull not possible\n";
            return;
        }
    
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
    
        std::cout << "The points in Convex Hull are:\n";
        while (!hull.empty())
        {
            std::cout << "(" <<( *hull.begin()).first << ", "
                << (*hull.begin()).second << ") ";
            hull.erase(hull.begin());
        }
    }

    std::pair<float, float> calculatePos(int index) {
        int posX = index % 8; 
        int posY = index / 8;  
        std::pair<float, float> t = std::make_pair(xInit + (GAP_CIRCLES_X * (posX + 1)), HEIGHT - (yInit + (GAP_CIRCLES_Y * (posY + 1)))); 

        std::pair<int, int> p = std::make_pair(posX, posY); 
        arrayOfPairs[compteurPair++] = p; 
        return t; 
    }

    std::string generateLines() {return "";}

    // Generates circles on the board if they are detected by the robot
    std::string generateCircles() {
        std::string svgData = ""; 

        for (int index = 0; index < SIZE_DATA; index++) {
            if (data[index] == 1) {
                std::pair<float, float> t = calculatePos(index);
                svgData += "<circle cx=\"" + std::to_string(t.first) + "\" cy=\"" + std::to_string(t.second) +  "\" stroke=\"black\" stroke-width=\"2\" r=\"10\" fill=\"grey\"/>";
            }
        }
        return svgData;
    }

    // Generates squares on the board
    std::string generateSquares() {
        std::string svgData = ""; 
        int posY = yInit; 
        for (int i = 0; i < 4; i++) {
            posY += 96; 
            int posX = xInit;
            for (int j = 0; j < 8; j++){
                posX += 106;
                svgData += "<rect x=\"" + std::to_string(posX) + "\" y=\"" + std::to_string(posY) + "\" width=\"5\" height=\"5\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>";
            }
        }
        return svgData;
    }

    // Generates the header of the svg file 
    std::string generateHeader() {
        std::string head = "<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1152 576\"> <rect x=\"96\" y=\"48\" width=\"960\" height=\"480\" stroke=\"black\" stroke-width=\"1\" fill=\"white\"/>";
        return head; 
    }

    // Generates the end of the svg file
    std::string generateEnd() {
        std::string end = "<text x=\"96\" y=\"36\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">section 06 -- équipe 145146 -- VROUMY</text>";     
        end += "<text x=\"96\" y=\"552\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">Aire 484 pouces carrés</text> "; 
        end += "</svg>";
        return end; 
    }

    void generateSVG() {
        std::ofstream fichier("test.svg");
        fichier << generateHeader();
        fichier << generateSquares();
        fichier << generateCircles(); 
        fichier << generateEnd(); 

        int n = 3; 
        printHull(arrayOfPairs, n);

        fichier.close();
    }

private: 
    int* data = new int[SIZE_DATA]; 
    std::pair<int, int> arrayOfPairs[3]; 
    int compteurPair = 0; 
    std::set<std::pair<int, int>> hull;
    float xInit = 96;
    float yInit = 48;
};