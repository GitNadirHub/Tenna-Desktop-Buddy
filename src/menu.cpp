#include "menu.hpp"

void Menu::open(sf::Vector2f pos)
{
    position = pos;
    isOpen = true;
} //test

void Menu::close()
{
    isOpen = false;
}

void Menu::draw(sf::RenderWindow& window, sf::Font& font)
{
    if (!isOpen) return;

    sf::RectangleShape bg({ width, itemHeight * (float)items.size() });
    bg.setPosition(position);
    bg.setFillColor(sf::Color(30, 30, 30, 240));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(2);
    window.draw(bg);

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font);
        text.setString(items[i].label);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(22);
        text.setPosition({ position.x + 10, position.y + i * itemHeight + 5 });
        window.draw(text);
    }
}

MenuAction Menu::handleClick(sf::Vector2f mouse)
{
    if (!isOpen) return MenuAction::None;

    sf::FloatRect bounds(position, { width, itemHeight * (float)items.size() });
    if (!bounds.contains(mouse)) return MenuAction::None;

    int index = (mouse.y - position.y) / itemHeight;
    if (index >= 0 && index < (int)items.size()) {
        return items[index].action;
    }

    return MenuAction::None;
}