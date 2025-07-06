#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <tlhelp32.h>
#include <SFML/audio.hpp>
#include "menu.hpp"
#include "intro.hpp"
#include "animator.hpp"
#include "resources.hpp"
#include "dialogue.hpp"
#include <random>
#include <iostream>

bool debugMode = false;

using namespace sf;

enum class TennaState
{
    idle,
    tpose,
    explode,
    freakout,
    dance1,
    dance2,
    tv_time
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
bool autoSpeak = true;

float tennaWidth = tenna.sprite.getTexture().getSize().x;
float tennaHeight = tenna.sprite.getTexture().getSize().y;

float scaleCorrection = 1.f; //used to scale the sprite to a desired height, so it remains consistent when changing the texture
float desiredHeight = 128; //desired height, mentioned above rahhhh

Menu menu;

void initialize(RenderWindow &window)
{

    initializeDialogue();

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
        {"       ~ Menu ~", },
        { "Close", MenuAction::Close },
        {"Idle", MenuAction::MenuIdle},
        {"T-Pose", MenuAction::Tpose},
        {"Dance", MenuAction::Dance},
		{"Speak", MenuAction::Speak},
        {"Auto speak: ON", MenuAction::ToggleAutoSpeak },
        {"Pluey", MenuAction::Pluey},    };

}

bool isProcessRunning(const std::wstring& processName)
{
    bool found = false;

    //Take a snapshot of all running processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    // Iterate over the process list :fire:
    if (Process32FirstW(snapshot, &pe))
    {
        do
        {
            if (processName == pe.szExeFile)
            {
                found = true;
                break;
            }
        } while (Process32NextW(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return found; //note to self: don't use this every frame, as commented above, it does quite a LOOOT
}

extern Sound snd_tenna_talk;


SoundBuffer buf_snd_dogtrill("res/snd/dogtrill.wav");
Sound snd_dogtrill(buf_snd_dogtrill); //click the menu title ten times

int randomRange(int lower, int higher)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(lower, higher);
    return dist(rng);
}

float randomRangeF(float lower, float higher)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(lower, higher);
    return dist(rng);
}

bool freakoutLastFrame = 0;

int dialogueAppearTime = randomRange(3, 7); //first appearance is always quicker

Clock processClock;

bool isAnnoyingDog = 0; //to annoying dog or to not annoying dog, that is the question

bool isUTorDT = 0;

void handleProcessInteractions()
{
    if (processClock.getElapsedTime().asSeconds() >= 5.0f)
    {
        processClock.restart();
        isAnnoyingDog = isProcessRunning(L"Annoying Dog Desktop Buddy.exe");
        if (!isAnnoyingDog)
        {
            isUTorDT = isProcessRunning(L"DELTARUNE.exe") || isProcessRunning(L"Undertale.exe");
        }
    }

    if (isAnnoyingDog)
    {
        tenna.state = TennaState::freakout;
        snd_tenna_talk.setPitch(randomRangeF(0.6f, 1.2f));
        if (snd_tenna_talk.getStatus() != Sound::Status::Playing)
            snd_tenna_talk.play();
        freakoutLastFrame = true;
    }
    else if (freakoutLastFrame)
    {
        snd_tenna_talk.setPitch(1.0f);
        snd_tenna_talk.stop();

        tenna.state = TennaState::idle;
        dialogueAppearTime = 15;

        freakoutLastFrame = false;
    }
    else if (isUTorDT)
    {
        tenna.state = TennaState::tv_time;
    }
}

int noneCount = 0;

bool handleDialogueLogic()
{

    if (tenna.state != TennaState::idle && tenna.state != TennaState::tpose && tenna.state != TennaState::tv_time
        && tenna.state!=TennaState::dance1 && tenna.state!=TennaState::dance2)
        return false;
    if (dialogueAppearTime != 0 && !autoSpeak)
        return false;

    static Clock dialogueClock;
    int elapsedTime = dialogueClock.getElapsedTime().asSeconds();
    static int dialogueEndTime = dialogueAppearTime + 10;

    if (elapsedTime > dialogueEndTime)
    {
        dialogueClock.restart();
        dialogueAppearTime = randomRange(0, 60);
        dialogueEndTime = dialogueAppearTime + 10;
        dialogueLineIndex++;
        return false;
    }

    if (elapsedTime > dialogueAppearTime)
    {
        return true;
    }
    return false;
}

Clock deltaClock;

Clock randomStateClock;

float secondsTillEvent = 25;

bool danceFromMenu = 0;

void randomState()
{

    if (tenna.state == TennaState::tpose || tenna.state == TennaState::explode || tenna.state == TennaState::freakout)
		return;

    if (danceFromMenu)
        return;

     if (randomStateClock.getElapsedTime().asSeconds() >= 10)
        {
            tenna.state = TennaState::idle;
            secondsTillEvent = randomRangeF(40, 120);
            randomStateClock.restart();
            return;
        }


    if (secondsTillEvent > randomStateClock.getElapsedTime().asSeconds())
        return;
    secondsTillEvent = randomRangeF(20, 50);
    randomStateClock.restart();
    int randomAction = randomRange(0, 3);
    if (randomAction <= 1)
        tenna.state = TennaState::idle;
    else if (randomAction <= 2)
        tenna.state = TennaState::dance1;
    else
        tenna.state = TennaState::dance2;
}

void draw(RenderWindow& window)
{
    Time elapsed = animationClock.getElapsedTime();

    float dt = deltaClock.restart().asSeconds();

    int currentFrame;

    bool showDialogue = debugMode || handleDialogueLogic();

    randomState();

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
        plueySprite.setPosition({ plueySprite.getPosition().x + 10, plueySprite.getPosition().y });
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
    case TennaState::freakout:
        tenna.sprite.setTexture(t_freakout);
        animateLooped(81, 128, 128, 0.001, elapsed, tenna.sprite);
        break;
    case TennaState::tv_time:
    {
        static Vector2f desiredPos = { 256.f, window.getSize().y * 1.f - tenna.sprite.getTexture().getSize().y/2.f};
        Vector2f deltaPos = desiredPos - tenna.position;
        if (deltaPos.y < 1.f)
        {
            tenna.position = desiredPos;
            tenna.sprite.setTexture(t_lookaway);
        }
        tenna.position += deltaPos * dt;
        break;
    }
    case TennaState::dance1:
        tenna.sprite.setTexture(t_dance0);
        animateLooped(93, 88, 146, 0.025, elapsed, tenna.sprite);
        break;
    case TennaState::dance2:
        tenna.sprite.setTexture(t_dance_cane);
        animateLooped(59, 123, 145, 0.03, elapsed, tenna.sprite);
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
    if (showDialogue)
	    dialogueDraw(window, tenna.position, tenna.state==TennaState::tv_time);
    window.display();
}

u_int danceCount = 1;


void coolStuff(RenderWindow &window)
{

    static Keyboard::Key konamiKeys[] =
    {
        Keyboard::Key::Up,
        Keyboard::Key::Up,
        Keyboard::Key::Down,
        Keyboard::Key::Down,
        Keyboard::Key::Left,
        Keyboard::Key::Right,
        Keyboard::Key::Left,
        Keyboard::Key::Right,
        Keyboard::Key::B,
        Keyboard::Key::A
    };

    static int konamiIndex = 0;

    if (konamiIndex >= 10)
    {
        tenna.state = TennaState::tpose;
        snd_friend_inside_me.play();
		debugMode = !debugMode;
        konamiIndex = 0;
    }
    else if (sf::Keyboard::isKeyPressed(konamiKeys[konamiIndex]))
    {
        konamiIndex++;
    }

    static Keyboard::Key theKeys[] =
    {
        Keyboard::Key::G,
        Keyboard::Key::A,
        Keyboard::Key::S,
        Keyboard::Key::T,
        Keyboard::Key::E,
        Keyboard::Key::R
    };

	static int theIndex = 0;

    if (theIndex >= 6)
    {
        window.close();
    }
    else if (sf::Keyboard::isKeyPressed(theKeys[theIndex]))
    {
        theIndex++;
    }

}

// You may not rest now. There are monsters nearby

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
        bool menuWasOpen = menu.isOpen;

        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

		coolStuff(window); //very epic

        // Open menu on new right-click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !rightPressedLastFrame)
        {
            menu.open(mousePosF);
            menuOpen = 1;
        }
        rightPressedLastFrame = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);


        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !leftReleasedLastFrame && menu.isOpen)
        {
            MenuAction action = menu.handleClick(mousePosF);
            menuWasOpen = menu.isOpen;
            menu.close();

            //HANDLE MENU ACTIONS
            danceFromMenu = action == MenuAction::Dance; // randomState() logic could stop dance early, even if user-requested

            switch (action)
            {
            case MenuAction::None:
                if (noneCount++ == 5)
                {
                    snd_dogtrill.play();
                    noneCount = 0;
                }
                break;
            case MenuAction::Close:
                tenna.state = TennaState::explode;
                break;
            case MenuAction::Tpose:
                tenna.state = TennaState::tpose;
                break;
            case MenuAction::MenuIdle:
                tenna.state = TennaState::idle;
                break;
            case MenuAction::Pluey:
                plueyActive = !plueyActive;
                snd_friend_inside_me.play();
                break;
			case MenuAction::Speak:
				dialogueAppearTime = 0; //instantly show dialogue, if not already showing
                break;
            case MenuAction::ToggleAutoSpeak:
                autoSpeak = !autoSpeak;
                if (autoSpeak)
                    menu.items[6].label = "Auto speak: ON";
                else
                    menu.items[6].label = "Auto speak: OFF";
				break;
            case MenuAction::Dance:
                if (danceCount == 1)
                {
                    menu.items[4].label = "Dance: Cane";
                    danceCount = 2;
                    tenna.state = TennaState::dance1;
                }
                else
                {
                    menu.items[4].label = "Dance: Cabbage";
                    danceCount = 1;
                    tenna.state = TennaState::dance2;
                }
                break;
            }
        }
        leftReleasedLastFrame = !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);


        if (Mouse::isButtonPressed(Mouse::Button::Left)  && !menuWasOpen)
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

    if (tenna.state != TennaState::tv_time && tenna.position.y > window.getSize().y - tenna.sprite.getTexture().getSize().y / 2.f)
    {
        if (snd_slide.getStatus() != Sound::Status::Playing && !isDragging)
            snd_slide.play();
        tenna.position.y -= 2.f + sin(timer); //goofy ahh appearance

    }

    tenna.sprite.setPosition(tenna.position);


    //timer += 0.025f;
    //tenna.sprite.setPosition({tenna.position.x + std::sin(timer) * 2.f, tenna.position.y});

    handleProcessInteractions();

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
        handleLogic(window);
        draw(window);
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main();
}

// You have reached the bottom of the code. You may now rest.