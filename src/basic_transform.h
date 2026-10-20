// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // methods...
    BasicTransform(sf::Vector2f s = {1, 1}, sf::Vector2f t = {0, 0}): scale(s), translation(t) {}

    sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        return p * scale + translation;
    }

    sf::Vector2f applyVector(const sf::Vector2f& p) const {
        return p * scale;
    }
    
    bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};
