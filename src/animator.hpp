#pragma once
#include <SFML/Graphics.hpp>

extern void animateLooped(int frameCount, int frameWidth, int frameHeight, float animationSpeed, sf::Time elapsedTime, sf::Sprite &sprite);

extern int animateIndexed(int frameCount, int frameWidth, int frameHeight, float animationSpeed, sf::Time elapsedTime, sf::Sprite &sprite);