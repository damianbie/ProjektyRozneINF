#include "Game.h"
#include "config.hpp"

#include <iostream>

Vector2f Game::gravicity = { 0.0f, 400.0f };
int Game::speed = 500;

Game::Game(std::shared_ptr<sf::RenderWindow> w, sf::VideoMode vm)
	:player({ 60, 46 }, "data/player.png", { (float)vm.width, (float)vm.height }), scoreAndTime(0,0)
{
	this->_wnd = w;
	this->_videoMode.x = (float)vm.width;
	this->_videoMode.y = (float)vm.height;
	//this->_view = new View({ (WINDOW_WIDTH/2), WINDOW_HEIGHT/2 }, { WINDOW_WIDTH, WINDOW_HEIGHT });
	//this->_wnd->setView(*_view);

	this->_clock.restart();
	this->_lastUpdate = sf::Time::Zero;

	if (!this->bg.loadFromFile("data/bg.png"))
	{
		_getch();
		exit(0);
	}
	if (!this->_lose.loadFromFile("data/lose.wav") ||
		!this->_collectPoint.loadFromFile("data/score.wav") ||
		!this->_tap.loadFromFile("data/tap.wav") ||
		!this->_hitToObject.loadFromFile("data/hit.wav"))
	{
		_getch();
		exit(1);
	}
	this->lose.setBuffer(_lose);
	this->collectPoint.setBuffer(_collectPoint);
	this->hitToObject.setBuffer(_hitToObject);
	this->tap.setBuffer(_tap);

	this->lose.setVolume(100);
	this->collectPoint.setVolume(100);
	this->hitToObject.setVolume(100);

	this->background.setTexture(&(this->bg));
	this->background.setSize({ (float)this->_wnd->getSize().x, (float)this->_wnd->getSize().y });
	this->background.setPosition(0, 0);

	this->gameState = NotStarted;
}
void Game::initGame()
{
	if (!this->bg.loadFromFile("data/bg.png"))
	{
		cerr << "Blad ladowania tekstury data/bg.png" << endl;
		exit(0);
	}
	if (!this->pipeHeadTexture.loadFromFile("data/head.png"))
	{
		cerr << "Blad ladowania tekstury data/head.png" << endl;
		exit(0);
	}
	if (!this->pipeBodyTexture.loadFromFile("data/body.png"))
	{
		cerr << "Blad ladowania tekstury data/body.png" << endl;
		exit(0);
	}
	if (!font.loadFromFile("data/Verdana.ttf"))
	{
		cerr << "Blad ladowania czcionki data/Verdana.ttf" << endl;
		exit(0);
	}


	this->_wnd->clear();
	this->_clock.restart();
	this->_mainClock.restart();

	this->gameState = GameState::LaunchGame;
	this->_lastUpdate = sf::Time::Zero;
	this->lastObjectSpawned = sf::seconds(0);
	this->nextSpawn = 2;
	this->loseMusicPlayed = false;
	cout << "     Game inited" << endl;
}
void Game::run()
{
	initGame();
	sf::Time loseTime = sf::Time::Zero;

	while (true)
	{
		if (this->gameState == GameState::LaunchGame)
		{
			float t = this->_mainClock.getElapsedTime().asSeconds();
			if (t > TIMETOSTARTGAME)
			{
				this->gameState = GameState::Run;
				cout << "Game started...." << endl;
			}
		}

		if (gameState == GameState::EndGame)
		{
			break;
		}
		else if (this->gameState == GameState::Pause)
		{
			cout << "Game paused" << endl;
			this->drawPauseScreen();
		}
		else if (this->gameState == GameState::Lose)
		{
			if (this->scoreAndTime.first == 0)
			{
				this->scoreAndTime.first = player.getScore();
				this->scoreAndTime.second = this->_mainClock.getElapsedTime().asSeconds();
			}
			if (!this->loseMusicPlayed)
			{
				lose.setVolume(20);
				lose.play();
				loseMusicPlayed = true;
				_wnd->clear();

				drawBackgrounds();
				drawLoseScreen();

				_wnd->display();
				cout << "=====Lose" << endl;
			}
				//lose screen
		}


		if (this->_lastUpdate.asSeconds() > (1.0f / FPS))
		{

			if (this->gameState == GameState::Run)
			{
				this->mainGame();
				if (this->gameState == GameState::Lose)
					loseTime = this->_mainClock.getElapsedTime();
			}

			lastGameUpdate = _mainClock.getElapsedTime();
			this->_lastUpdate = sf::Time::Zero;
			this->_clock.restart();
		}
		Event ev;
		while (_wnd->pollEvent(ev))
		{
			typedef GameState GS;
			if (ev.type == Event::KeyReleased)
			{
				if (gameState == GS::Pause)
				{
					gameState = GS::Run;
					break;
				}
				else if (gameState == GS::Lose)
				{
					if(this->_mainClock.getElapsedTime().asSeconds() - loseTime.asSeconds() > DELAY_AFTER_LOSE)
						gameState = GS::EndGame;
				}
				else if (ev.key.code == Keyboard::P)
				{
					gameState = GameState::Pause;
				}
				else if (ev.key.code == sf::Keyboard::Escape)
				{
					if (gameState == GS::Run)
					{
						gameState == GS::Pause;
					}
					else if (gameState == GS::Pause)
					{
						gameState = GS::EndGame;
					}
				}
				else if (ev.key.code == Keyboard::Space && gameState == GameState::Run)
				{
					player.jump();
					tap.play();
				}
				else if (ev.key.code == Keyboard::Z)
				{
					;//
				}
			}
			switch (ev.type)
			{
			case Event::Closed:
				_wnd->close();
				exit(0);
				break;
			}
		}
		this->_lastUpdate = this->_clock.getElapsedTime();
	}

	cout << "End game" << endl;
}
void Game::mainGame()
{
	srand(time(NULL));
	float centerX = (this->_wnd->getSize().x / 2);

	player.update((_mainClock.getElapsedTime() - lastGameUpdate), centerX);

	if (player.isDie())
	{
		gameState = GameState::Lose;
		return;
	}

	if (this->lastObjectSpawned.asSeconds() + nextSpawn <= _mainClock.getElapsedTime().asSeconds())
	{
		lastObjectSpawned = _mainClock.getElapsedTime();
		nextSpawn = ((int)rand() % 2) + ((int)rand() % 1)+ 1;

		int sx = _wnd->getSize().x + rand() % (_wnd->getSize().x/3);
		int sy = ((_wnd->getSize().y / 3) - (rand() % 40));
		int ma = (rand() % 100) + 240;
										// startX startY margin
		this->objects.push_back(Obstacle(sx, sy, ma, &pipeHeadTexture, &pipeBodyTexture));
	}

	for (int it = 0; it < objects.size(); it++)
	{
		objects[it].update(this->_mainClock.getElapsedTime() - lastGameUpdate);
		if (objects[it].isOkay() == false)
		{
			objects.erase(objects.begin() + it);
			continue;
		}

		if (objects[it].collision(player.getPos().x ,player.getPos().y, player.getSize().x, player.getSize().y))
		{
			// collision 
			hitToObject.play();
			cout << "Collision" << endl;
			player.die();
		}
		if (player.getPos().x + player.getSize().x >= objects[it].getBodyPos().x + objects[it].getBodySize().x && objects[it].isScored() == false)
		{
			objects[it].score();
			player.addScore();
			collectPoint.play();
			cout << "Score added:" << player.getScore() << endl;

		}
	}

	// drawing...
	_wnd->clear();

	// moving background
	drawBackgrounds();

	for (std::vector<Obstacle>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		it->draw(_wnd);
	}

	drawGUI();
	draw();
}
void Game::drawPauseScreen()
{
	Font flappy;
	flappy.loadFromFile("data/FlappyBirdy.ttf");

	int x = this->_wnd->getSize().x / 2;
	int y = this->_wnd->getSize().y / 2;

	Text title;
	title.setString("Pauza");
	title.setFont(flappy);
	title.setColor(sf::Color::Black);
	title.setStyle(sf::Text::Bold);
	title.setCharacterSize(140);

	x -= (title.getLocalBounds().width / 2);
	title.setPosition(x, y - 300);

	this->_wnd->clear();

	drawBackgrounds();
	for (std::vector<Obstacle>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		it->draw(_wnd);
	}

	this->player.draw(_wnd);

	this->_wnd->draw(title);
	title.setString("Nacisnij dowolny przycisk");
	title.setPosition(_wnd->getSize().x/2 - (title.getGlobalBounds().width/2) ,this->_wnd->getSize().y/2);

	this->_wnd->draw(title);
	this->_wnd->display();

}
void Game::drawGUI()
{
	if (gameState == GameState::Run)
	{
		Text sc;
		sc.setCharacterSize(40);
		sc.setFont(font);
		sc.setColor(sf::Color::Black);
		
		std::string s = "Punkty: ";
		s += std::to_string(player.getScore());

		sc.setString(s);
		sc.setPosition(20, 20);
		_wnd->draw(sc);

		sc.setPosition(20, 30 + sc.getLocalBounds().height);
		sc.setString("Czas gry:" + std::to_string((int) this->_mainClock.getElapsedTime().asSeconds()) + "s");
		_wnd->draw(sc);
	}
}
void Game::draw()
{
	// draw all elements on display...

	//this->_wnd->draw();
	this->player.draw(_wnd);

	this->_wnd->display();
}
void Game::drawLoseScreen()
{
	for (std::vector<Obstacle>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		it->draw(_wnd);
	}

	Font f1;
	f1.loadFromFile("data/Verdana.ttf");

	Font f2;
	f2.loadFromFile("data/FlappyBirdy.ttf");

	Text t;
	t.setFont(f2);
	t.setCharacterSize(200);
	t.setString("Game over");
	t.setColor(sf::Color::Black);
	t.setPosition(_wnd->getSize().x /2 - t.getGlobalBounds().width/2, _wnd->getSize().y/16);
	_wnd->draw(t);

	t.setString("Czas gry: " + std::to_string((int)_mainClock.getElapsedTime().asSeconds()) + "s");
	t.setFont(f1);
	t.setCharacterSize(42);
	t.setPosition(_wnd->getSize().x / 2 - t.getGlobalBounds().width / 2, _wnd->getSize().y / 3 +50);
	_wnd->draw(t);

	t.setString("Punkty: " + std::to_string(player.getScore()));
	t.setFont(f1);
	t.setCharacterSize(42);
	t.setPosition(_wnd->getSize().x / 2 - t.getGlobalBounds().width / 2, _wnd->getSize().y / 3 + 100);
	_wnd->draw(t);
	player.draw(_wnd);
}
void Game::drawBackgrounds()
{
	if (this->gameState == GameState::Run)
	{
		int copyOfx = 0;
		int _x = this->background.getPosition().x;
		_x -= (_mainClock.getElapsedTime().asSeconds() - lastGameUpdate.asSeconds()) * this->speed;
		copyOfx = _x;
		this->background.setPosition(_x, this->background.getPosition().y);

		this->_wnd->draw(background);

		_x = this->background.getPosition().x + this->background.getGlobalBounds().width;
		this->background.setPosition(_x, this->background.getPosition().y);
		this->_wnd->draw(background);

		if (copyOfx <= -(this->_wnd->getSize().x))
			copyOfx = 0;

		this->background.setPosition(copyOfx, this->background.getPosition().y);
	}
	else
	{
		int copyOfx = 0;
		int _x = this->background.getPosition().x;
		copyOfx = _x;
		this->background.setPosition(_x, this->background.getPosition().y);

		this->_wnd->draw(background);

		_x = this->background.getPosition().x + this->background.getGlobalBounds().width;
		this->background.setPosition(_x, this->background.getPosition().y);
		this->_wnd->draw(background);

		if (copyOfx <= -(this->_wnd->getSize().x))
			copyOfx = 0;

		this->background.setPosition(copyOfx, this->background.getPosition().y);
	}
}
std::pair<int, int> Game::getScoreAndTime()
{
	return this->scoreAndTime;
}