#include "animator.hpp"

void animateLooped(int frameCount, int frameWidth, int frameHeight, float animationSpeed, sf::Time elapsedTime, sf::Sprite &sprite)
{ //makes it gifable
    int currentFrame = (int)(elapsedTime.asSeconds() / animationSpeed) % frameCount;
    sprite.setTextureRect(sf::IntRect({ currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
}

int animateIndexed(int frameCount, int frameWidth, int frameHeight, float animationSpeed, sf::Time elapsedTime, sf::Sprite &sprite)
{
    int currentFrame = (int)(elapsedTime.asSeconds() / animationSpeed);
    sprite.setTextureRect(sf::IntRect({ currentFrame * frameWidth, 0 }, { frameWidth, frameHeight }));
    return currentFrame;
}