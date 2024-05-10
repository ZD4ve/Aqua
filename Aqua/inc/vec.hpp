#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>

template <typename num>
concept arithmetic = std::integral<num> or std::floating_point<num>;

struct vec {
   private:
    static bool almostEQ(double a, double b) {
        return std::abs(a - b) < 1.0E-10;
    }

   public:
    double x{0};
    double y{0};

    vec() = default;
    template <typename num>
        requires arithmetic<num>
    explicit vec(num x, num y) {
        this->x = static_cast<double>(x);
        this->y = static_cast<double>(y);
    }
    template <typename num>
        requires arithmetic<num>
    explicit vec(num n) {
        this->x = static_cast<double>(n);
        this->y = static_cast<double>(n);
    }
    template <typename num>
    vec(sf::Vector2<num> v) {
        this->x = static_cast<double>(v.x);
        this->y = static_cast<double>(v.y);
    }
    template <typename num>
    operator sf::Vector2<num>() const {
        return sf::Vector2(static_cast<num>(x), static_cast<num>(y));
    }
    template <typename num>
        requires arithmetic<num>
    friend vec operator*(vec v, num i) {
        return vec(v.x * i, v.y * i);
    }
    template <typename num>
        requires arithmetic<num>
    friend vec operator*(num i, vec v) {
        return vec(v.x * i, v.y * i);
    }
    template <typename num>
        requires arithmetic<num>
    vec operator/(num i) const {
        return vec(x / i, y / i);
    }
    template <typename T>
    vec operator+(sf::Vector2<T> v) const {
        return vec(x + v.x, y + v.y);
    }
    template <typename T>
    friend vec operator+(sf::Vector2<T> v1, vec v2) {
        return vec(v1.x + v2.x, v1.y + v2.y);
    }
    vec operator+(vec v) const {
        return vec(x + v.x, y + v.y);
    }
    vec &operator+=(vec v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    vec operator-(vec v) const {
        return vec(x - v.x, y - v.y);
    }
    template <typename T>
    vec operator-(sf::Vector2<T> v) const {
        return vec(x - v.x, y - v.y);
    }
    template <typename T>
    friend vec operator-(sf::Vector2<T> v1, vec v2) {
        return vec(v1.x - v2.x, v1.y - v2.y);
    }
    vec &operator-=(vec v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    bool operator==(vec v) const {
        return almostEQ(x, v.x) && almostEQ(y, v.y);
    }
    bool operator!=(vec v) const {
        return !operator==(v);
    }

    double len() const {
        return std::hypot(x, y);
    }
    vec norm() const {
        double l = len();
        if (l < 1.0E-10) return vec(std::sin(std::rand() / 1000.0), std::cos(std::rand() / 1000.0));
        return *this / l;
    }
    bool wholeEQ(vec v) const {
        auto a = whole();
        auto b = v.whole();
        return a.x == b.x && a.y == b.y;
    }
    sf::Vector2<ssize_t> whole() const {
        return {static_cast<ssize_t>(std::floor(x)), static_cast<ssize_t>(std::floor(y))};
    }
};