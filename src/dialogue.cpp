#include "dialogue.hpp"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <chrono>
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
"Without my TV, \nIm lost in transmission.",
"What?!\nI'd do wordart,\nbut the budget was\nused up on the\nvoice acting!",
"Wordart? \nI hardly know her!\n",
"My TV is not a great\nlistener, \nbut it sure is a great talker!",
"Ralsei plush! Fill\nhim with liquid,\nthrow him against the\nwall!",
"Darn!! I\novercooked the\nwater!!",
"Oops!! No!!\nI keep breaking\nthe eggs!!",
"Tonight on PAY-PER-VIEW:\nMe. Staring at the wall.",
"Broadcasting live from\nthe inside of your fridge!",
"We're experiencing technical difficulties.\nPlease scream into the void.",
"The dog is in the vents again.",
"That dog took my job.\nAnd my lunch.",
"You call this a DESKTOP?",
"He's back...\nWith the eggs.",
"You ever see a dog\ndo taxes?\nNo?\nLucky you.",
"I see youre running Windows...bad choice.\nWhat's Windows, anyway?\nSome kind of TV program?",
"You're not real.\nI made you up\nto cope with the static.",
"I see what you're doing.\nAnd I approve.",
"Don't think about eggs.\nToo late.",
"I know your secrets.\nEspecially the one about the spoon.",
"If you dream of me,\nI dream back.",
"Stop blinking.\nYou're wasting frames.",
"I once knew a man\nnamed Table.\nHe wasn't very stable.",
"Behold!\nA perfectly normal message.",
"One of these lines\nis secretly a recipe.",
"Blink once for yes.\nTwice for spaghetti.",
"They took the microwave, Kris.\nTHE MICROWAVE!",
"He said he'd come back...\nHe never returned from the store.",
"I trusted you.\nThen you opened the Recycle Bin.",
"Where is ASRIEL?\nWhat?! COLLEGE?!\nBut you can learn anything on TV!!",
"KRIS!\nThe ratings are low.\nDo something chaotic!",
"They said I was 'retro'...\nThen they threw me in the attic.",
"You ever got replaced\nby a smart fridge?\nI have.",
"Is it still nostalgia\nif no one remembers?",
"Look at me!\nPlease?\nJust a little glance?",
"The ratings...\nI can feel them dropping!",
"Don't change the channel!\nI can still entertain you!",
"This is my moment!\nLights! Camera! Sadness!",
"Back in MY day,\nwe had scheduled programming!",
"I'm still broadcasting,\nare you still watching?",
"Welcome back to Channel Me!",
"Hope you brought popcorn!\nBecause I'm about to POP OFF!",
"Behold!\nA television with zero channels\nbut infinite charisma!",
"Smile for the static!",
"THIS JUST IN:\nYou're awesome!",
"Live from the living room:\nIts ME!",
"Are you not entertained!?\nNo? Well I have 47 backup plans!",
"BREAKING NEWS:\nIm breaking...dance.",
"Im powered by drama\nand possibly ghosts!",
"Hold onto your remotes!\nThis episodes going off the rails!",
"Someone get the confetti!\nIm making an entrance!",
"Guess who just got upgraded\nwith... nothing!\nBut I FEEL upgraded!",
"I run on attention\nand lukewarm soup!",
"Coming up next:\nA MYSTERIOUS CRASH\nin your hallway!",
"Warning:\nMy volume control is emotional.",
"Im not yelling,\nIm projecting!",
"Did someone say ratings?\nBecause I just SPIKED.",
"I am the broadcast,\nthe whole broadcast,\nand nothing but the broadcast!",
"Stand back!\nI do my own stunts!",
"You cant mute me!\nIve eaten the button!",
"Todays forecast:\nLoud with a chance of fabulous!",
"If I freeze,\njust pretend its dramatic tension.",
"Every time you blink,\nI get closer to a reboot!",
"You cant skip this scene,\nIm in ALL of them!",
"Im the star,\nthe sidekick,\nand the commercial break!",
"I once hosted a cooking show.\nIt ended in bad weather and tears!",
"CHANNEL SURFERS BEWARE:\nI BITE.",
"Quiet on set!\nIm about to monologue!",
"My face was made\nfor standard definition!",
"My energy bill?\nEmotionally expensive.",
"I swallowed a VHS\nand now I speak in rewinds!",
"MY SIGNAL IS\nILLEGALLY STRONG!",
"EVERY CHANNEL\nIS JUST ME SCREAMING!",
"Ive been on hold\nfor 12 years.\nTHE MUSIC NEVER STOPS.",
"Did you know I can\npick up radio signals?\nIm basically a DJ!",
"I'M A CRT, BABY!\nBUILT TO LAST AND\nTO BLAST!",
"FLAT SCREENS?\nMORE LIKE FLAT PERSONALITIES!",
"Im 50 pounds of FUN\nand five inches of CHAOS!",
"My pixels are THICC.",
"I hum when Im angry.\nWhich is always.",
"Don't put a magnet near me.",
"I EAT REMOTES\nFOR BREAKFAST.",
"My screen curves like your spine.\nSERIOUSLY, CORRECT YOUR POSTURE!",
"WARNING:\nI MAY CONTAIN LEAD\nAND UNHINGED ENERGY.",
"You call it outdated.\nI call it VINTAGE RAGE.",
"I broadcast at 480i of FURY.",
"THE ONLY HIGH DEFINITION I NEED\nIS HIGH DRAMA!",
"I'M TOO ANALOG\nTO APOLOGIZE!",
"I WAS BUILT\nBEFORE YOUR MOM\nFINISHED DIAL-UP!",
"Do I buzz when I'm on?\nYES.\nThat's ME PULLING POWER FROM THE ETHER.",
"I OVERHEAT WITH LOVE!",
"My back is HUGE.\nMy power is IMMEASURABLE.",
"People used to gather 'round me\nand... what was I going to say?",
"HDMI?\nI ONLY SPEAK IN COAXIAL!",
"I played cartoons\nthat made kids cry.\nYOU'RE NEXT.",
"Dont put me in a landfill.\nILL HAUNT THE RECYCLING CENTER!",
"I was BORN in a basement\nand RAISED on CHANNEL 3.",
"THE ONLY FLAT THING ABOUT ME\nIS MY PATIENCE!",
"My screen can reflect your face\nand your REGRET.",
"Glass? Yes.\nClass? DOUBLE YES.",
"Im a whole entertainment center.\nYOU are just a viewer.",
"I once electrocuted a man\nby EXISTING NEAR HIM.",
"I dont DO Wi-Fi,\nI DO WHY-CRY.",
"I WILL OUTLIVE\nEVERY STREAMING SERVICE!"


};

int dialogueLineIndex = 0;

static Font font("res/fonts/sb.ttf");
Text dialogueText(font);

//int i = strings.size()-1; 

void initializeDialogue()
{
	strings.push_back("Did you know?\nThis game holds exactly\n" + to_string(strings.size()+2) + " lines.\nI'm one of them.");
	strings.push_back("This message has\nthe chance of \nappearing " + to_string(100.f /( strings.size()+1)) + "% \nof the time.\n");
	
	default_random_engine engineRandom;
	engineRandom.seed(chrono::system_clock::now().time_since_epoch().count());


	shuffle(strings.begin(), strings.end(), engineRandom);
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
float elapsed=0;


void dialogueDraw(RenderWindow& window, const Vector2f tennaPos)
{
	elapsed = dialogueClock.restart().asMilliseconds();

	static int lastLineIndex = -1; //elapsed does not correctly reset, so we forcefully reset it as shown below

	if (dialogueLineIndex != lastLineIndex)
		elapsed = 0;


	//DEBUG
	if (Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		if (!keyPressedLastFrame)
		{
			elapsed = 0;
			dialogueLineIndex++;
			if (dialogueLineIndex >= strings.size())
			{
				dialogueLineIndex = 0;
			}
			keyPressedLastFrame = true;
		}
	}
	else
		keyPressedLastFrame = false;
	//DEBUG



	float slowDownFactor = 1.f + ((strings[dialogueLineIndex][textIndex] == '\n') * 5.f) +
								 (isPunctuation(strings[dialogueLineIndex][textIndex]) && !isPunctuation(strings[dialogueLineIndex][textIndex+1]) * 15.f);

	if (elapsed == 0.f)
		textIndex = 0;
	else
		textIndex = max(textIndex, textIndex + elapsed/(25.f*slowDownFactor));

	if (textIndex >= strings[dialogueLineIndex].size())
		textIndex = strings[dialogueLineIndex].size(), snd_tenna_talk.pause();
	else if (snd_tenna_talk.getStatus()!=Sound::Status::Playing)
		snd_tenna_talk.play();

	//if (slowDownFactor > 1.f)
	//	snd_tenna_talk.pause();


	Text invisibleText = dialogueText;

	string loadedText = strings[dialogueLineIndex].substr(0, textIndex);

	invisibleText.setString(strings[dialogueLineIndex]); //the final text size
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

	lastLineIndex = dialogueLineIndex;
}