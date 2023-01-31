#pragma once
#include "config.hpp"
#include "Player.h"

class Game
{
public:
	Game(std::shared_ptr<sf::RenderWindow> w, sf::VideoMode vm);
	void run();

	std::pair<int, int> getScoreAndTime();

	static Vector2f gravicity;
	static int speed; //pixels per second
private:
	void initGame();
	void draw();
	void drawGUI();
	void mainGame();
	void drawPauseScreen();
	void drawBackgrounds();
	void drawLoseScreen();

	std::pair<int, int> scoreAndTime;

	Player								player;
	GameState							gameState;
	std::vector<Obstacle>				objects;

	std::shared_ptr<sf::RenderWindow>	_wnd;
	sf::View*							_view;
	sf::Vector2f						_videoMode;

	sf::Texture							pipeHeadTexture;
	sf::Texture							pipeBodyTexture;
	sf::Texture							bg;
	sf::RectangleShape					background;
	sf::Font							font;

	sf::Clock							_clock;
	sf::Clock							_mainClock;
	sf::Time							_lastUpdate;
	sf::Time							lastGameUpdate;
	sf::Time							lastObjectSpawned;

	sf::Sound							lose;
	sf::Sound							collectPoint;
	sf::Sound							hitToObject;
	sf::Sound							tap;
	sf::SoundBuffer						_tap;
	sf::SoundBuffer						_lose;
	sf::SoundBuffer						_collectPoint;
	sf::SoundBuffer						_hitToObject;

	bool								loseMusicPlayed;
	float								nextSpawn;

};