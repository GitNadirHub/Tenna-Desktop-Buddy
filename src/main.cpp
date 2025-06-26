#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <SFML/audio.hpp>
#include "menu.hpp"
#include "intro.hpp"
#include "animator.hpp"
#include <iostream>

using namespace sf;

Texture t_idle("res/img/idle.png");
Texture t_tpose("res/img/tpose.png");
Texture t_explode("res/img/explode.png");
Texture t_pluey("res/img/pluey.png");

SoundBuffer buf_snd_slide("res/snd/slide.wav");
Sound snd_slide(buf_snd_slide);

SoundBuffer buf_snd_boom("res/snd/boom.wav");
Sound snd_boom(buf_snd_boom);

SoundBuffer buf_snd_friend_inside_me("res/snd/friend_inside_me.wav");
Sound snd_friend_inside_me(buf_snd_friend_inside_me);

Font font("res/fonts/deltarune.ttf");

enum TennaState
{
    idle,
    state_tpose,
    explode
};

struct Tenna
{
    Sprite sprite;
    Vector2f position;
    TennaState state = idle;
    Tenna(sf::Texture& texture) : sprite(texture), position(960.f, 540.f)
    {
        sprite.setPosition(position);
    }
};

Sprite plueySprite(t_pluey);

Tenna tenna(t_idle);

Clock animationClock;

bool plueyActive = false;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Tenna :D", sf::Style::None);
    window.setFramerateLimit(144);


    HWND hwnd = window.getNativeHandle(); 
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //makes it on top of anything
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); 

    drawIntro(window);

    float tennaWidth = tenna.sprite.getTexture().getSize().x;
    float tennaHeight = tenna.sprite.getTexture().getSize().y;

    tenna.sprite.setOrigin({ tennaWidth / 2.f, tennaHeight / 2.f });

    float timer = 0.f;

	bool lastMousePressed = false;
    bool isDragging = false;

    snd_slide.setVolume(30.f);

	Vector2f dragOffset = { 0.f, 0.f };

    Menu menu;
    menu.items =
    {
        { "Close", Close },
        {"Idle", MenuIdle},
        {"T-Pose", Tpose},
        {"Pluey", Pluey}
    };

	float scaleCorrection = 1.f;
    float desiredHeight = 128;
    
	bool menuOpen = 0, rightPressedLastFrame = 0, leftReleasedLastFrame = 0;

    while (window.isOpen())
    {

        TennaState lastState = tenna.state;

        tennaWidth = tenna.sprite.getTextureRect().size.x;
        tennaHeight = tenna.sprite.getTextureRect().size.y;

        tenna.sprite.setOrigin({ tennaWidth / 2.f, tennaHeight / 2.f });

		Vector2i mousePos = Mouse::getPosition(window);

        Vector2f mousePosF = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

        bool flipped = mousePos.x < tenna.position.x; //this is already the origin (tenna.position.x)

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Open menu on new right-click
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !rightPressedLastFrame) {
                menu.open(mousePosF);
                menuOpen = 1;
            }
            rightPressedLastFrame = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

            if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !leftReleasedLastFrame && menu.isOpen)
            {
                MenuAction action = menu.handleClick(mousePosF);
                menu.close();
                switch (action)
                {
                case Close:
                    tenna.state = explode;
					break;
                case Tpose:
                    std::cout << "tpose";
                    tenna.state = state_tpose;
                    break;
                case MenuIdle:
                    tenna.state = idle;
                    break;
				case Pluey: 
                    plueyActive = !plueyActive;
                    snd_friend_inside_me.play();
                    break;
                }
            }
            leftReleasedLastFrame = !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);


            if (Mouse::isButtonPressed(Mouse::Button::Left))
            {
                if (lastMousePressed)
                {
                    isDragging = true;
                }
                else if (tenna.sprite.getGlobalBounds().contains(mousePosF))
                {
                    lastMousePressed = true;
					dragOffset = { tenna.position.x - mousePosF.x, tenna.position.y - mousePosF.y };
                }
            }
            else
				lastMousePressed = false, isDragging = false;

        }
        if (isDragging)
        {

            tenna.position = mousePosF + dragOffset;

            sf::FloatRect bounds = tenna.sprite.getGlobalBounds();
            sf::Vector2u winSize = window.getSize();
        }

        if (tenna.position.y < tenna.sprite.getTexture().getSize().y/2.f)
            tenna.position.y = tenna.sprite.getTexture().getSize().y / 2.f;

        if (tenna.position.y > window.getSize().y - tenna.sprite.getTexture().getSize().y / 2.f)
        {
            if (snd_slide.getStatus() != Sound::Status::Playing && !isDragging)
				snd_slide.play();
            tenna.position.y -= 2.f + sin(timer); //goofy ahh appearance
            
        }

        tenna.sprite.setPosition(tenna.position);


        //timer += 0.025f;
        //tenna.sprite.setPosition({tenna.position.x + std::sin(timer) * 2.f, tenna.position.y});

        if (tenna.state != lastState)
        {
            animationClock.restart();
		}   

        Time elapsed = animationClock.getElapsedTime();

        int currentFrame;

        switch (tenna.state)
        {
        case idle:
            tenna.sprite.setTexture(t_idle, true);
            break;
        case state_tpose:
			tenna.sprite.setTexture(t_tpose); //second argument resets the texture rect
            animateLooped(15, 131, 149, 0.1, elapsed, tenna.sprite);
            break;
        case explode:
            tenna.sprite.setTexture(t_explode);
            currentFrame = animateIndexed(20, 240, 426, 0.1, elapsed, tenna.sprite);
            if (currentFrame == 13)
            {
				snd_boom.play();
            }
            else if (currentFrame >19) //last frame
            {
                while (snd_boom.getStatus() == Sound::Status::Playing)
                {
                    //wait for sound to finish
                }
                window.close();
            }
			break;
        default:
            break;
        }
        
        desiredHeight = 266.f;
        scaleCorrection = desiredHeight / tenna.sprite.getTexture().getSize().y;

        tenna.sprite.setScale({ (flipped && tenna.state == idle ? 1.f : -1.f ) * scaleCorrection, scaleCorrection });

        window.clear();
		window.draw(tenna.sprite);
        if (menu.isOpen)
            menu.draw(window, font);
        if (plueyActive)
        {
            float plueyX = plueySprite.getPosition().x + sin(Mouse::getPosition(window).x);
			float plueyY = plueySprite.getPosition().y + sin(Mouse::getPosition(window).y);
            plueySprite.setPosition({plueyX, plueyY});
            window.draw(plueySprite);
            int t = elapsed.asSeconds();
            if (t % 17 == 0)
                plueySprite.setPosition(tenna.position), snd_friend_inside_me.play();
		}
        window.display();
    }
}
