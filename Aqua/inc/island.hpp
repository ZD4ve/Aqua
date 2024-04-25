#pragma once
#include <SFML/Graphics.hpp>

#include "vec.hpp"

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
        bool operator()(vec cord) const;
    };

   private:
    sf::Sprite canvasS;
    sf::Texture canvasT;
    sf::Shader shader;
    vec mapSize;
    Map map;

   public:
    explicit Island(vec mapSize);
    void draw(sf::RenderTarget &target);
    const Map &getMap() { return map; }
    vec getMapSize() { return mapSize; }
};

}  // namespace aq