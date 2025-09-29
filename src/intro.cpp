#include "intro.hpp"
#include "animator.hpp"
#include <SFML/Audio.hpp>

using namespace sf;

Texture tv_time1("res/img/tv_time1.png"), tv_time2("res/img/tv_time2.png"), tv_time3("res/img/tv_time3.png"), tv_time4("res/img/tv_time4.png");
//These must be not a direct constructor (like Texture::Texture)
//yes, that worked before, but the Linux build uses g++ sooooo... I have to suffer

Texture t_tv_time[4] = { tv_time1, tv_time2, tv_time3, tv_time4 };
Sprite tv_time(t_tv_time[0]);

static SoundBuffer buf_snd_boom("res/snd/boom.wav");
static Sound snd_boom(buf_snd_boom);

SoundBuffer buf_snd_tv_time("res/snd/tv_time.wav");
Sound snd_tv_time(buf_snd_tv_time);

const unsigned int chunkFrameCount = 24;

Clock animClock;

void drawIntro(sf::RenderWindow& window)
{
	int chunk = 0;

	tv_time.setScale({ 2.f, 2.f });

	sf::Vector2u windowSize = window.getSize();
	tv_time.setPosition(
		{ windowSize.x / 2.f - (426 * 2.f) / 2.f,
		windowSize.y / 2.f - (240 * 2.f) / 2.f }
	);


	//snd_tv_time.getAttenuation(); //should do something idk
	snd_tv_time.setVolume(15.f);
	snd_tv_time.play();

	int prevChunk = -1;

	while (window.isOpen()) //INTRO
	{


		while (const std::optional<sf::Event> event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		//DEBUG SKIP INTRO
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			snd_tv_time.stop();
			return;
		}
		//DEBUG SKIP INTRO



		auto elapsed = animClock.getElapsedTime();


		if (chunk >= 4)
			break;

		if (chunk != prevChunk)
			tv_time.setTexture(t_tv_time[chunk]);
		prevChunk = chunk;


		int index = animateIndexed(chunkFrameCount, 426, 240, 0.07, elapsed, tv_time);
		if (index == chunkFrameCount - 1)
		{
			chunk++, index = 0;
			animClock.restart();
		}

		if (chunk == 3 && index == 3)
			snd_boom.play();

		window.clear(sf::Color::Transparent);
		window.draw(tv_time);
		window.display();
	}
}