#include "island.hpp"

using namespace aq;

Island::Island(sf::Vector2u mapSize) : mapSize(mapSize) {
    if (!sf::Shader::isAvailable()) throw std::runtime_error("Shaders are not available on this machine!");
    canvasT.create(mapSize.x, mapSize.y);
    canvasS = sf::Sprite(canvasT);
    if (!shader.loadFromFile("src/perlin.frag", sf::Shader::Fragment)) throw std::runtime_error("Could not load shader!");
    std::srand(std::time(nullptr));
    shader.setUniform("u_seed", sf::Glsl::Vec2(std::rand() % 5000 - 2500, std::rand() % 5000 - 2500));
    shader.setUniform("u_octaves", 5);
    shader.setUniform("u_gridSize", 500.0F);
    shader.setUniform("u_amplitude", 2.0F);
    shader.setUniform("u_bw_mode", 0.0F);
    shader.setUniform("col_low_water", sf::Glsl::Vec4(sf::Color(0, 26, 51)));
    shader.setUniform("col_high_water", sf::Glsl::Vec4(sf::Color(0, 102, 204)));
    shader.setUniform("col_low_sand", sf::Glsl::Vec4(sf::Color(246, 215, 176)));
    shader.setUniform("col_high_sand", sf::Glsl::Vec4(sf::Color(255, 191, 146)));
    shader.setUniform("col_low_grass", sf::Glsl::Vec4(sf::Color(65, 152, 10)));
    shader.setUniform("col_high_grass", sf::Glsl::Vec4(sf::Color(19, 109, 21)));
    shader.setUniform("u_water_level", 0.6F);
    shader.setUniform("u_sand_level", 0.7F);
}

void Island::draw(sf::RenderTarget &target) {
    shader.setUniform("u_resolution", sf::Glsl::Vec2(target.getSize()));
    shader.setUniform("u_top_left", sf::Glsl::Vec2(target.mapPixelToCoords({0, 0})));
    shader.setUniform("u_bottom_right", sf::Glsl::Vec2(target.mapPixelToCoords(sf::Vector2i(target.getSize()))));
    target.draw(canvasS, &shader);
}
