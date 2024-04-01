#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "force.hpp"

namespace aq {

class Fish {
   private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::vector<Force *> forces;
    
    float vision;
    sf::Sprite sp;

   public:
    Fish() = default;
    Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision);
    Fish(const Fish& rhs);
    Fish &operator=(const Fish& rhs);
    sf::Vector2f getLocation() const {return position;}
    bool canSee(Fish &nb) const;
    void draw(sf::RenderTarget &target);
    template<typename iterator>
    void move(sf::Time dt, iterator begin, iterator end){
        for (iterator it = begin; it != end; ++it)
        {
            Fish &nb = *it;
            if(&nb == this) continue;
            if(!canSee(nb)) continue;
            for (auto &&f : forces) 
                f->accum(nb);
        }
        for (auto &&f : forces)
        {
            f->finalize();
            velocity+=f->getSum()*float(dt.asMicroseconds()/1000.0);
        }
        position+=velocity;
    }
    ~Fish();

};
}  // namespace aq