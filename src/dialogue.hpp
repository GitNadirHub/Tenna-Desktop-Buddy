#pragma once
#include <SFML/Graphics.hpp>

void initializeDialogue();

void dialogueDraw(sf::RenderWindow& window, const sf::Vector2f tennaPos);

extern int dialogueLineIndex;