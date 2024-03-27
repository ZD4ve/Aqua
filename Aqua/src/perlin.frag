/**
 * @brief Simple 2D perlin noise shader
 * @details Code based on the the Perlin noise wikipedia page: https://en.wikipedia.org/wiki/Perlin_noise
 * @namespace GLSL::NOISE
 * @class PerlinNoise
 */
#version 150

//* @cond
precision mediump float;
layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 fragColor;
//* @endcond

// Needs to be set once
uniform vec2 u_seed;             ///< Seed used as offset
uniform int u_octaves;           ///< Number of patterns to sum
uniform float u_gridSize;        ///< Size of the grid
uniform float u_amplitude;       ///< Start amlitude of the noise

uniform float u_water_level;     ///< Threshold for water [0, 1]
uniform float u_sand_level;      ///< Threshold for sand [0, 1]

uniform vec4 col_low_water;      ///< Color for deep water
uniform vec4 col_high_water;     ///< Color for shallow water
uniform vec4 col_low_sand;       ///< Color for low sand
uniform vec4 col_high_sand;      ///< Color for high sand
uniform vec4 col_low_grass;      ///< Color for low grass
uniform vec4 col_high_grass;     ///< Color for high grass

// Needs to be set each frame
uniform vec2 u_resolution;       ///< Size of the window
uniform vec2 u_top_left;         ///< Top left corner of the visible area
uniform vec2 u_bottom_right;     ///< Bottom right corner of the visible area

/**
 * @brief Smoothly interpolates between two values
 */
float interpolate(float a, float b, float w) {
    return (b - a) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a;
}

/**
 * @brief Caps a value between [0, 1]
 */
float cap(float value) {
    return min(1.0, max(0.0, value)); 
}

/**
 * @brief Computes a pseudo random gradient vector for a given integer coordinate
 * @return Vector with length 1
 */
vec2 randomGradient(ivec2 cord) {
    uint w = 32u; // 32 bit
    uint s = w / 2u; // rotation width
    uint a = uint(cord.x), b = uint(cord.y);
    a *= 3284157443u; b ^= a << s | a >> w-s;
    b *= 1911520717u; a ^= b << s | b >> w-s;
    a *= 2048419325u;
    float random = float(a) * (3.14159265 / float(~(~0u >> 1))); // in [0, 2*Pi]
    return vec2(cos(random), sin(random));
}

/**
 * @brief Computes the dot product of a random gradient vector and a given position
  */
float dotGridGradient(ivec2 cord, vec2 pos) {
    vec2 gradient = randomGradient(cord);
    vec2 delta = pos - cord;
    return dot(gradient, delta);
}

/**
 * @brief 2D Perlin noise
 * @param pos Position in 2D space
 * @return [-1, 1]
 */
float perlin(vec2 pos) {
    pos += u_seed;

    // Determine grid cell coordinates
    ivec4 cell = ivec4(ivec2(floor(pos)), ivec2(floor(pos)+1));

    // Determine interpolation weights
    vec2 delta = pos - cell.xy;

    // Calculate the value of the four corners
    vec4 corners = vec4(
        dotGridGradient(cell.xy, pos),
        dotGridGradient(cell.zy, pos),
        dotGridGradient(cell.xw, pos),
        dotGridGradient(cell.zw, pos)
    );

    return interpolate(
        interpolate(corners.x, corners.y, delta.x),
        interpolate(corners.z, corners.w, delta.x),
        delta.y
        ); 
}

/**
 * @brief Computes a fractal sum of perlin noise
 * @return [0, 1]
 */
float fractalNoise(vec2 pos) {
    float grid_size = u_gridSize;
    float ampl = u_amplitude;
    float height = 0.0;
    for (int i = 0; i < u_octaves; i++) {
        height += perlin(pos.xy/grid_size) * ampl;
        grid_size /= 2;
        ampl /= 2;
    }
    return cap(height*0.5+0.5);
}

/**
 * @brief Computes a color based on the height
 * @param height in [0, 1]
 */
vec4 colorFromHeight(float height) {
    if (height < u_water_level) {
        return mix(col_low_water, col_high_water, height/u_water_level);
    } else if (height < u_sand_level) {
        return mix(col_low_sand, col_high_sand, (height-u_water_level)/(u_sand_level-u_water_level));
    } else {
        return mix(col_low_grass, col_high_grass, (height-u_sand_level)/(1.0-u_sand_level));
    }
}

/**
 * @brief Main function
 */
void main() {
    vec2 relative = mix(u_top_left, u_bottom_right, gl_FragCoord.xy/u_resolution);
    float height = fractalNoise(relative);
    fragColor = colorFromHeight(height);
} 