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
        bool waterAt(vec cord) const;
    };

   private:
    sf::Sprite canvasS;
    sf::Texture canvasT;
    sf::Shader shader;
    vec mapSize;
    Map map;
    const sf::Color bgColor = sf::Color(19, 109, 21);

   public:
    explicit Island(vec mapSize);
    void draw(sf::RenderTarget &target);
    const Map &getMap() const { return map; }
    vec getMapSize() const { return mapSize; }
    sf::Color getBGcolor() const { return bgColor; }
};

}  // namespace aq