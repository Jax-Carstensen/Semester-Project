#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;

class Sound {
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	string fileName;
public:
	Sound() {}
	Sound(string newFileName) {
		setSound(newFileName);
	}
	~Sound() {}
	void setSound(string newFileName) {
		fileName = newFileName;
		if (buffer.loadFromFile("./sounds/" + newFileName + ".wav"))
			cout << newFileName << ".wav" << " was succesfully loaded" << endl;
		sound.setBuffer(buffer);
		sound.stop();
	}
	void play() {
		if (!sound.Playing) {
			cout << "Playing" << endl;
			sound.play();
		}
		else
			cout << "The sound was busy!" << endl;
	}
	string getFileName() {
		return fileName;
	}
};

#endif