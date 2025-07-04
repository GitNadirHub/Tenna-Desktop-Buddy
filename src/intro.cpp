#include "intro.hpp"
#include "animator.hpp"
#include <SFML/Audio.hpp>

using namespace sf;

Texture t_tv_time[4] = {Texture::Texture(("res/img/tv_time1.png")),
						Texture::Texture(("res/img/tv_time2.png")), 
						Texture::Texture(("res/img/tv_time3.png")), 
						Texture::Texture(("res/img/tv_time4.png")), };
Sprite tv_time(t_tv_time[0]);

static SoundBuffer buf_snd_boom("res/snd/boom.wav");
static Sound snd_boom(buf_snd_boom);

SoundBuffer buf_snd_tv_time("res/snd/tv_time.wav");
Sound snd_tv_time(buf_snd_tv_time);

const unsigned int chunkFrameCount = 24;

Clock animClock;

void drawIntro(sf::RenderWindow &window)
{
	int chunk = 0;

	tv_time.setScale({ 2.f, 2.f });
	tv_time.setPosition({ window.getSize().x / 4.f, window.getSize().y / 4.f });

	//snd_tv_time.getAttenuation(); //should do something idk
	snd_tv_time.setVolume(15.f);
	snd_tv_time.play();
	
	int prevChunk = -1;

	while (window.isOpen()) //INTRO
	{

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
		if (index == chunkFrameCount-1)
		{
			chunk++, index = 0;
			animClock.restart();
		}

		if (chunk == 3 && index == 3)
			snd_boom.play();

		window.clear();
		window.draw(tv_time);
		window.display();
	}
}