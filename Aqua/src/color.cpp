#include "color.hpp"

#include <cmath>
using namespace aq;

sf::Color Color::HSLtoRGB(double H, double S, double L) {
    while (H < 0) H += 360;
    while (H >= 360) H -= 360;
    double C = (1 - std::abs(2 * L - 1)) * S;
    double H1 = H / 60;
    double X = C * (1 - std::abs(std::fmod(H1, 2.0) - 1));
    double R1 = 0, G1 = 0, B1 = 0;
    switch (static_cast<int>(H1)) {
        case 0:
            R1 = C;
            G1 = X;
            B1 = 0;
            break;
        case 1:
            R1 = X;
            G1 = C;
            B1 = 0;
            break;
        case 2:
            R1 = 0;
            G1 = C;
            B1 = X;
            break;
        case 3:
            R1 = 0;
            G1 = X;
            B1 = C;
            break;
        case 4:
            R1 = X;
            G1 = 0;
            B1 = C;
            break;
        case 5:
            R1 = C;
            G1 = 0;
            B1 = X;
            break;
    }
    double m = L - C / 2;
    return sf::Color(255 * (R1 + m), 255 * (G1 + m), 255 * (B1 + m));
}