#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <SFML/audio.hpp>
#include "menu.hpp"
#include "intro.hpp"
#include "animator.hpp"
#include "resources.hpp"

using namespace sf;

enum class TennaState
{
    idle,
    tpose,
    explode
};

struct Tenna
{
    Sprite sprite;
    Vector2f position;
    TennaState state = TennaState::idle;
    Tenna(sf::Texture& texture) : sprite(texture), position(960.f, 540.f)
    {
        sprite.setPosition(position);
    }
};

Texture t_pluey("res/img/pluey.png"); //no idea why this cant be defined in resources.cpp, it just doesn't appear
// so we define it here. Oh, how insane my life has become. Pluey, what have you done to me?
Sprite plueySprite(t_pluey);

Tenna tenna(t_idle);

Clock animationClock; //used for elapsed time in, well, animations (read the name)

bool plueyActive = false;
bool flipped = false; //used to flip the sprite when the mouse is on the left side of tenna

float tennaWidth = tenna.sprite.getTexture().getSize().x;
float tennaHeight = tenna.sprite.getTexture().getSize().y;

float scaleCorrection = 1.f; //used to scale the sprite to a desired height, so it remains consistent when changing the texture
float desiredHeight = 128; //desired height, mentioned above rahhhh

Menu menu;

void initialize(RenderWindow &window)
{
    window.setFramerateLimit(144);

    HWND hwnd = window.getNativeHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //makes it on top of anything
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE); // get the current extended window style
	SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED); // set the extended style to include WS_EX_LAYERED which allows transparency
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); //make pure black transparent

    drawIntro(window); 

    snd_slide.setVolume(30.f);
    menu.items =
    {
        {"   - Menu -", },
        { "Close", Close },
        {"Idle", MenuIdle},
        {"T-Pose", Tpose},
        {"Pluey", Pluey}
    };

}

void draw(RenderWindow& window)
{
    Time elapsed = animationClock.getElapsedTime();

    int currentFrame;

    switch (tenna.state)
    {
    case TennaState::idle:
        tenna.sprite.setTexture(t_idle, true);
        break;
    case TennaState::tpose:
        tenna.sprite.setTexture(t_tpose); //second argument resets the texture rect
        animateLooped(15, 131, 149, 0.1, elapsed, tenna.sprite);
        break;
    case TennaState::explode:
        tenna.sprite.setTexture(t_explode);
        currentFrame = animateIndexed(20, 240, 426, 0.1, elapsed, tenna.sprite);
        if (currentFrame == 13)
        {
            snd_boom.play();
        }
        else if (currentFrame > 19) //last frame
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

    tenna.sprite.setScale({ (flipped && tenna.state == TennaState::idle ? 1.f : -1.f) * scaleCorrection, scaleCorrection });

    window.clear();
    window.draw(tenna.sprite);
    if (menu.isOpen)
        menu.draw(window, font);
    if (plueyActive)
    {
        float plueyX = plueySprite.getPosition().x + sin(Mouse::getPosition(window).x);
        float plueyY = plueySprite.getPosition().y + sin(Mouse::getPosition(window).y);
        plueySprite.setPosition({ plueyX, plueyY });
        window.draw(plueySprite);
        int t = elapsed.asSeconds();
        if (t % 17 == 0 && snd_friend_inside_me.getStatus() != Sound::Status::Playing)
        {
            plueySprite.setPosition(tenna.position);
            snd_friend_inside_me.play();
        }
    }
    window.display();
}

void handleLogic(RenderWindow &window)
{

    static float timer = 0.f;
    static bool lastMousePressed = false;
    static bool isDragging = false;

    Vector2f dragOffset = { 0.f, 0.f };


    static bool menuOpen = 0, rightPressedLastFrame = 0, leftReleasedLastFrame = 0;

    //The static keyword is used to keep the state across function calls, without making them global variables.
    //Just initialised once at the start of the program :p

    TennaState lastState = tenna.state;

    tennaWidth = tenna.sprite.getTextureRect().size.x;
    tennaHeight = tenna.sprite.getTextureRect().size.y;

    tenna.sprite.setOrigin({ tennaWidth / 2.f, tennaHeight / 2.f });

    Vector2i mousePos = Mouse::getPosition(window);

    Vector2f mousePosF = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };

    flipped = mousePos.x < tenna.position.x; //this is already the origin (tenna.position.x)

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
                tenna.state = TennaState::explode;
                break;
            case Tpose:
                tenna.state = TennaState::tpose;
                break;
            case MenuIdle:
                tenna.state = TennaState::idle;
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

    if (tenna.position.y < tenna.sprite.getTexture().getSize().y / 2.f)
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
}

int main()
{
    auto window = RenderWindow(VideoMode({1920u, 1080u}), "Tenna :D", sf::Style::None);

    initialize(window);

    while (window.isOpen())
    {
        draw(window);
        handleLogic(window);
    }
}
