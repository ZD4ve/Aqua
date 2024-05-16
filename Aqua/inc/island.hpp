#pragma once
#include <SFML/Graphics.hpp>

#include "vec.hpp"

namespace aq {

/**
 * @brief Responsible for handleing the island shader
 */
class Island {
   public:
    /**
     * @brief A non-copyable class that represents the map of the islands
     */
    struct Map {
       private:
        sf::Image data;
        void setImage(sf::Image &img) { data = img; }
        friend class Island;

       public:
        Map() = default;
        Map(const Map &) = delete;
        Map &operator=(const Map &) = delete;
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
    /**
     * @brief Loads the openGL(GLSL) shader
     * @throws if an error occurs while loading and compiling the shader
     */
    explicit Island(vec mapSize);
    void draw(sf::RenderTarget &target);
    const Map &getMap() const { return map; }
    vec getMapSize() const { return mapSize; }
    sf::Color getBGcolor() const { return bgColor; }
};

}  // namespace aq