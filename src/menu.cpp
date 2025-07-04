#include "menu.hpp"

#include <SFML/Audio.hpp>

using namespace sf; //i give up, im adding this, not gonna make this consistent tho, now it's a mess

SoundBuffer buf_snd_hover("res/snd/select.wav");
Sound snd_hover(buf_snd_hover);

void Menu::open(sf::Vector2f pos)
{
    position = { pos.x, std::min(pos.y, 760.f) };
    isOpen = true;
} //test

void Menu::close()
{
    isOpen = false;
}

static Clock animationClock;

void Menu::draw(sf::RenderWindow& window, sf::Font& font)
{
    if (!isOpen) 
        return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = { mousePos.x * 1.f, mousePos.y * 1.f}; //dum dum 

    sf::RectangleShape bg({ width, itemHeight * (float)items.size() });
    bg.setPosition(position);
    bg.setFillColor(sf::Color(30, 30, 30, 240));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(5);

    sf::RectangleShape shadow(bg.getScale());
    shadow.setPosition(bg.getPosition() + Vector2f(5,5));
    shadow.setFillColor(sf::Color(1, 2, 1, 255));
    window.draw(shadow);

    window.draw(bg);

    for (int i = items.size()-1; i>=0; i--) //to draw menu title thing last, works like a charm
    {
        sf::Text text(font);
        text.setString(items[i].label);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(22);
        text.setPosition({ position.x + 10, position.y + i * itemHeight + 5 });

        text.setStyle(Text::Regular);
        text.setOutlineThickness(2.f);
        text.setOutlineColor(Color(4, 4, 4, 180));

        if (i!=0)
        {
            sf::RectangleShape highlight({ width, itemHeight });
            highlight.setPosition({ position.x, position.y + i * itemHeight });

            if (highlight.getGlobalBounds().contains(mousePosF))
            {
                highlight.setFillColor(sf::Color(80, 80, 80, 180));
                window.draw(highlight);
                sf::RectangleShape aura({ width, itemHeight + 8 });
                aura.setOrigin({ 4, 4 });
                aura.setPosition({ position.x+5, position.y + i * itemHeight });
                float t = animationClock.getElapsedTime().asSeconds();
                int glow = 100 + std::sin(t * 3 + i) * 20;
                aura.setFillColor(Color(120, 150, 255, glow));
                window.draw(aura);

				text.setScale({ 1.1f, 1.1f });
            }
        }
        if (i==0) // i==0 -> that's the menu text thingy, yes, it's a button thing cuz im lazy
        {
            sf::RectangleShape divider({ width, 3.f });
            divider.setFillColor(sf::Color(255, 255, 255, 255));
            divider.setPosition({ position.x, position.y + itemHeight - 1 });
            window.draw(divider);
        }
        else
        {
            sf::RectangleShape divider({ width, 2.f });
            divider.setFillColor(sf::Color(255, 255, 255, 50));
            divider.setPosition({ position.x, position.y + itemHeight * i - 1 });
            window.draw(divider);
        }

        window.draw(text);
    }

    sf::RectangleShape outline({ width, itemHeight * (float)items.size() });
    outline.setPosition(position);
    outline.setFillColor(Color::Transparent);
    outline.setOutlineColor(Color::White);
    outline.setOutlineThickness(5);
    window.draw(outline);

}

MenuAction Menu::handleClick(sf::Vector2f mouse)
{
    if (!isOpen) return MenuAction::None;

    sf::FloatRect bounds(position, { width, itemHeight * (float)items.size() });
    if (!bounds.contains(mouse)) 
        return MenuAction::None;

    int index = (mouse.y - position.y) / itemHeight;

    if (index == 0)
        return MenuAction::None;

    if (snd_hover.getStatus() != Sound::Status::Playing)
        snd_hover.setVolume(30.f), snd_hover.setPlayingOffset(milliseconds(300)), snd_hover.play();

    if (index >= 0 && index < (int)items.size())
    {
        return items[index].action;
    }

    return MenuAction::None;
}