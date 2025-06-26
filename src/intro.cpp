#include "intro.hpp"
#include "animator.hpp"

using namespace sf;

Texture t_tv_time[4] = {Texture::Texture(("res/img/tv_time1.png")),
						Texture::Texture(("res/img/tv_time2.png")), 
						Texture::Texture(("res/img/tv_time3.png")), 
						Texture::Texture(("res/img/tv_time4.png")), };
Sprite tv_time(t_tv_time[0]);

const unsigned int chunkFrameCount = 24;

Clock animClock;

void drawIntro(sf::RenderWindow &window)
{

	int chunk = 0;

	tv_time.setScale({ 2.f, 2.f });
	FloatRect spriteRect = tv_time.getLocalBounds();
	tv_time.setPosition({ window.getSize().x / 4.f, window.getSize().y / 4.f });

	while (window.isOpen()) //INTRO
	{
		auto elapsed = animClock.getElapsedTime();
		
		int index = animateIndexed(chunkFrameCount, 426, 240, 0.07, elapsed, tv_time);
		if (index == chunkFrameCount)
		{
			chunk++, index = 0;
			animClock.restart();
		}

		if (chunk >= 4)
			break;
		tv_time.setTexture(t_tv_time[chunk]);

		window.clear();
		window.draw(tv_time);
		window.display();
	}
}