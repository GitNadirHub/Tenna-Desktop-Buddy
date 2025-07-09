#pragma once

#include <SFML/Window.hpp>

#ifdef __linux__
void makeAlwaysOnTop(sf::Window& window);
#endif
