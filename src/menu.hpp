#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum MenuAction {
    None,
    Close,
    Tpose,
    MenuIdle
};

struct MenuItem {
    std::string label;
    MenuAction action;
};

struct Menu {
    std::vector<MenuItem> items;
    sf::Vector2f position;
    bool isOpen = false;
    float width = 140.f;
    float itemHeight = 30.f;

    void open(sf::Vector2f pos);
    void close();
    void draw(sf::RenderWindow& window, sf::Font& font);
    MenuAction handleClick(sf::Vector2f mouse);
};