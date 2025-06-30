#include "resources.hpp"

using namespace sf;

Texture t_idle("res/img/idle.png");
Texture t_tpose("res/img/tpose.png");
Texture t_explode("res/img/explode.png");

SoundBuffer buf_snd_slide("res/snd/slide.wav");
Sound snd_slide(buf_snd_slide);

SoundBuffer buf_snd_boom("res/snd/boom.wav");
Sound snd_boom(buf_snd_boom);

SoundBuffer buf_snd_friend_inside_me("res/snd/friend_inside_me.wav");
Sound snd_friend_inside_me(buf_snd_friend_inside_me);

Font font("res/fonts/deltarune.ttf");