#pragma once
#include <SFML/Graphics.hpp>

namespace aq {

class Island {
   private:
    sf::Sprite canvasS;
    sf::Texture canvasT;
    sf::Shader shader;
    const sf::Vector2u &mapSize;

   public:
    Island(const sf::Vector2u &mapSize);
    void draw(sf::RenderTarget &target);
};

}  // namespace aq