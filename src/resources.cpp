#include "resources.hpp"

using namespace sf;

Texture t_idle("res/img/idle.png");
Texture t_tpose("res/img/tpose.png");
Texture t_explode("res/img/explode.png");
Texture t_dance_cane("res/img/dance_cane.png");
Texture t_dance0("res/img/dance0.png");
//Texture t_dance1("res/img/dance1.png");
//Texture t_dance2("res/img/dance2.png");
//Texture t_dance3("res/img/dance3.png");
//Texture t_dance4("res/img/dance4.png");
//Texture t_dance5("res/img/dance5.png");
Texture t_freakout("res/img/freakout.png");
Texture t_lookaway("res/img/lookaway.png");

SoundBuffer buf_snd_slide("res/snd/slide.wav");
Sound snd_slide(buf_snd_slide);

SoundBuffer buf_snd_boom("res/snd/boom.wav");
Sound snd_boom(buf_snd_boom);

SoundBuffer buf_snd_friend_inside_me("res/snd/friend_inside_me.wav");
Sound snd_friend_inside_me(buf_snd_friend_inside_me);

Font font("res/fonts/deltarune.ttf");