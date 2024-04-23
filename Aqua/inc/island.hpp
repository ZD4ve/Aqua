#pragma once
#include <SFML/Graphics.hpp>

namespace aq {

class Island {
   public:
    struct Map {
       private:
        sf::Image data;
        void setImage(sf::Image &img) { data = img; }
        friend class Island;

       public:
        Map() = default;
        Map(const Map &) = delete;
        Map &operator=(const Map &) = delete;
        /**
         * @brief Can fish go to cord
         * @param cord cord on map
         * @return true if water, false is island
         */
        bool operator()(sf::Vector2f cord) const;
    };

   private:
    sf::Sprite canvasS;
    sf::Texture canvasT;
    sf::Shader shader;
    sf::Vector2u mapSize;
    Map map;

   public:
    explicit Island(sf::Vector2u mapSize);
    void draw(sf::RenderTarget &target);
    const Map &getMap() { return map; }
    sf::Vector2u getMapSize() { return mapSize; }
};

}  // namespace aq