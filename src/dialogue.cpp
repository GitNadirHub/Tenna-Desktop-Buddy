#include "dialogue.hpp"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
using namespace sf;
using namespace std;

SoundBuffer buf_snd_tenna_talk("res/snd/tenna_talk.wav");
Sound snd_tenna_talk(buf_snd_tenna_talk);


vector<string> strings = {"Hello world!",
"Lalala~ \nTime to wake up and \nsmell the pain",
"HEAR THAT WHINE!?\nTHAT'S YOUR CRT ASKING FOR A WALK!!",
"BIG [Hyperlink blocked] \nDOWN THE LANE!",
"Bepis.",
"Vrei sa pleci, dar nu ma nu ma iei\nNu ma nu ma iei\nNu ma nu ma nu ma iei\nChipul tau si dragostea din tei\nMi-amintesc de ochii tai ei",
"My eggs - husband ? \nWhat do you want to know about him ? \nActually, I don't want to talk about this...",
"His eggs-husband?\nYou mean me?\nNO??? He had another eggs-husband????",
"That trashy mailman...\n",
"That trashy mailman...\nI miss him.",
"I'd let you talk too,\nbut the censors would kill me!",
"WHAT THE ?#!$%? IS AN EMAIL?",
"Mike, play the VHS!\nMike...?",
"The family's fighting again...",
"I love TV!   ",
"It's TV time!   ",
"I love TV! \nSay it! \nSAY IT! ",
"Now where the heck \nis my debit card?",
"December? Oh, you mean Dess?\nShe was a really sweet girl.\nShe especially loved singing this song... What was it's name again?\nSomething with a bat... and cats\nWhat? Cats aren't mentioned in the song? I swear they were tho...",
"I LOVE TV!",
"Kris!\nWhat do you mean \nyou're not Kris?",
"Don't touch that dial! \nAnd don't stop those smiles! \n",
"I'm just a toyyyy toooo youuuu my loveeeeeeeeee\n",
"MIKE, PLAY THE VHS!!! \nMike???",
"IT'S NOW TIME FOR OUR FEATURE PRESENTATION!\n",
"COMING STRAIGHT \nFROM YOUR HOUSE!",
"COMING STRAIGHT \nFROM *YOUR* HOUSE!",
"COMING! \n",
"FEACHER",
"He's the One! (coming) \n",
"The King of Only! \n",
"He's GROOVY \nand never GLOOBY! \n",
"You can't get this \nfrom an EGG!",
"You can't get THIS \nfrom a DOG.",
"How I hate that dog...\n",
"God forbid you \nOPEN THAT \nANNOYING DOG.",
"I burnt the water.",
"The sensation of \nof your screen! ",
"The show that \nmakes you \nSCREAM! ",
"The sensation of \nof your screen \nThe show that \nmakes you \nSCREAM! ",
"Desktop pet? \nNo pet is on top of any desk here! ",
"The HECK is a desktop pet? \nI'm not on your desk!",
"I'm probably looking away \nfrom my words, \nam I not, Kris?",
"\n\n\nThis is LONG\n\n\n",
"\nTHIS \nTEXTBOX\nIS\n\nTOO\nLONG\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",
"I have been \nwriting these \nfor quite a while now \nsend help. ",
"Help? \nI don't need help, \nI need a new job!",
"I think I have \nseen this one before.\n",
"Have you ever \nseen a dog \nwith a gun?",
"Have you ever \nmet that dog? \nHe's a menace, Kris!\nDon't trust him!",
"Undertale? \nI hardly know her!\n",
"Deltarune? \nI hardly know her!\n",
"Kris, \nwill I become a Big Shot?\nThat mailman promised...",
"So... \nI guess I have to \nwrite a lot of these, huh?\n",
"So... \nYou... \nCome here often?\n",
"Hey, Kris! \nDid you know that \nI can read your mind? \nI can see your thoughts, \nyour dreams, your desires... \nI can even see your \nsearch history! \nFOR LEGAL REASONS, \nI CAN'T TELL YOU \nWHAT I SEE IN YOUR MIND.\n",
"Entertainment!",
"Entertainment! \n",
"Don't touch that dial! \nAnd don't stop those smiles! \n",
"KRIS?!\nLOOK AT THESE \nRATINGS! \nTHEY'RE OFF THE CHARTS!\n",
"Without my TV, \nI’m lost in transmission.",
"What?!\nI'd do wordart,\nbut the budget was\nused up on the\nvoice acting!",
"Wordart? \nI hardly know her!\n",
"My TV is not a great\nlistener, \nbut it sure is a great talker!",
"Ralsei plush! Fill\nhim with liquid,\nthrow him against the\nwall!",
"Darn!! I\novercooked the\nwater!!",
"Oops!! No!!\nI keep breaking\nthe eggs!!"
};

static Font font("res/fonts/sb.ttf");
Text dialogueText(font);

//int i = strings.size()-1; 
int i = 0;

void initializeDialogue()
{
	strings.push_back("T his message has\nthe chance of \nappearing " + to_string(100.f / strings.size()+1) + "% \nof the time.\n");

	shuffle(strings.begin(), strings.end(), std::default_random_engine());
	dialogueText.setString(strings[0]);
	dialogueText.setCharacterSize(8);
	dialogueText.setFillColor(Color::Color(1, 1, 1, 255));


}

bool keyPressedLastFrame = false;

Clock dialogueClock;

float textIndex = 0;

bool isPunctuation(char c)
{
	if (c == '!' || c == '?' || c == '.' || c == ',') //this aint the only punctuation, but it's the only one i give a BURGENTRUCKUNG about
		return true;
	return false;
}

void dialogueDraw(RenderWindow& window, const Vector2f tennaPos)
{
	float elapsed = dialogueClock.restart().asMilliseconds();

	//DEBUG
	if (Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		if (!keyPressedLastFrame)
		{
			elapsed = 0;
			i++;
			if (i >= strings.size())
			{
				i = 0;
			}
			keyPressedLastFrame = true;
		}
	}
	else
		keyPressedLastFrame = false;
	//DEBUG



	float slowDownFactor = 1.f + ((strings[i][textIndex] == '\n') * 5.f) + 
								 (isPunctuation(strings[i][textIndex]) && !isPunctuation(strings[i][textIndex+1]) * 15.f);

	if (elapsed == 0.f)
		textIndex = 0;
	else
		textIndex = max(textIndex, textIndex + elapsed/(25.f*slowDownFactor));

	if (textIndex >= strings[i].size())
		textIndex = strings[i].size(), snd_tenna_talk.pause();
	else if (snd_tenna_talk.getStatus()!=Sound::Status::Playing)
		snd_tenna_talk.play();

	//if (slowDownFactor > 1.f)
	//	snd_tenna_talk.pause();


	Text invisibleText = dialogueText;

	string loadedText = strings[i].substr(0, textIndex);

	invisibleText.setString(strings[i]); //the final text size
	dialogueText.setString(loadedText);

	float width = invisibleText.getGlobalBounds().size.x;
	float height = invisibleText.getGlobalBounds().size.y;


	Vector2f pos = { tennaPos.x - 70.f, tennaPos.y - 100.f };

	dialogueText.setPosition(pos);
	invisibleText.setPosition(pos);

	dialogueText.setOrigin({ width, 0 });
	invisibleText.setOrigin({ width, 0 });


	RectangleShape background({ width + 15.f, height +15.f });
	RectangleShape background2({ width + 30.f, height });

	background.setOrigin({background.getSize().x, 0});
	background2.setOrigin(background.getOrigin());
	background.setPosition({ dialogueText.getPosition().x+5.f, dialogueText.getPosition().y - 15.f });
	background2.setPosition({background.getPosition().x - 7.f, background.getPosition().y+7});

	window.draw(background);
	window.draw(background2);
	window.draw(dialogueText);
}