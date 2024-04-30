#pragma once
#include <SFML/Graphics/Color.hpp>
#include <random>

namespace aq {

class Color {
   public:
    double H;
    double S;
    double L;
    double r;

   private:
    /**
     * @details Equations from https://en.wikipedia.org/wiki/HSL_and_HSV
     */
    static sf::Color HSLtoRGB(double H, double S, double L);
    static double distribution(double x) {
        return x * std::abs(x);
    }
    static inline double random() {
        return (std::rand() / static_cast<double>(RAND_MAX)) * 2 - 1;
    }

   public:
    /**
     * @param H Hue [0,360)
     * @param S Saturation [0,1]
     * @param L Lightness [0,1]
     * @param range allowed +- from hue
     */
    Color(double H, double S, double L, double range = 0) : H(H), S(S), L(L), r(range) {}

    /**
     * @brief Generate a random color centered with a distribution
     * @param hue_center [0,360)
     * @param hue_range allowed +- from center
     * @param color_randomness randomness of rgb generated from the returned color
     */
    static Color randomColor(double hue_center, double hue_range, double saturation, double lightness, double color_randomness = 0) {
        double hue = hue_center + hue_range * distribution(random());
        return Color(hue, saturation, lightness, color_randomness);
    }

    operator sf::Color() {
        return HSLtoRGB(H + r * distribution(random()), S, L);
    }
};
}  // namespace aq
