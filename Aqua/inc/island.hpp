#pragma once
#include <SFML/Graphics.hpp>

namespace aq {

class Island {
   private:
    sf::Sprite canvasS;
    sf::Texture canvasT;
    sf::Shader shader;
    sf::Vector2u mapSize;

   public:
    explicit Island(sf::Vector2u mapSize);
    void draw(sf::RenderTarget &target);
    sf::Vector2u getMapSize() { return mapSize; }
};

}  // namespace aq