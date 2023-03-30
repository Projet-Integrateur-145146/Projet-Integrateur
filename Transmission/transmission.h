#include <iostream>
#include <fstream>

class Transmission
{
public: 
    Transmission() {}; 
    ~Transmission() { delete[] data; }

    std::pair<float, float> calculatePos(int index) {
        int posX = index % 8; 
        int posY = index / 8; 
        std::pair<float, float> t = std::make_pair(xInit * (posX + 1) , yInit * (posY + 1)); 
        return t; 
    }

    std::string generateLines() {return "";}

    std::string generateCircles() {return "";}

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

    std::string generateHeader() {
        std::string head = "<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1152 576\"> <rect x=\"96\" y=\"48\" width=\"960\" height=\"480\" stroke=\"black\" stroke-width=\"1\" fill=\"white\"/>";
        return head; 
    }

    std::string generateEnd() {
        std::string end = "<text x=\"96\" y=\"36\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">section 06 -- équipe 145146 -- VROUMY</text> ";     
        end += "<text x=\"96\" y=\"552\" font-family=\"arial\" font-size=\"20\" fill=\"blue\">Aire 484 pouces carrés</text> "; 
        end += "</svg>";
        return end; 
    }

    void generateSVG() {
        std::ofstream fichier("test.svg");
        fichier << generateHeader();
        fichier << generateSquares();
        fichier << generateEnd(); 
        fichier.close();
    }

private: 
    int* data = new int[32]; 
    float xInit = 96;
    float yInit = 48;
};