#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture) {
        mTexture = &texture;
        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
        mMinPointTex = {0, 0};
        mMaxPointTex = sf::Vector2f(texture.getSize());
        mOrigin = {0, 0};
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint) {
        mTexture = &texture;
        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
        mMinPointTex = texMinPoint;
        mMaxPointTex = texMaxPoint;
        mOrigin = {0, 0};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (mTexture == nullptr) return;
        recomputeVertexArray();
        states.texture = mTexture;
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {
        sf::Vector2f{0, 0},
        sf::Vector2f{0, 1},
        sf::Vector2f{1, 0},

        sf::Vector2f{1, 0},
        sf::Vector2f{0, 1},
        sf::Vector2f{1, 1}
    };

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        if (mTexture == nullptr) return;
        sf::Vector2f size = mMaxPointTex - mMinPointTex;
        for (int i = 0; i < 6; i++) {
            sf::Vector2f texOffset = DEFAULT_VERTICES[i] * size;

            mVertexArray[i].texCoords = mMinPointTex + texOffset;
            mVertexArray[i].position = mTransform.applyPoint(texOffset - mOrigin);
        }
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
