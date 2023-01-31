#pragma once
#include "config.hpp"

class Player
{
public:
	Player(sf::Vector2f size, std::string text, sf::Vector2f ws);
	~Player();

	void draw(std::shared_ptr<sf::RenderWindow> w);
	void update(sf::Time dt, float newX);
	void jump();
	void die();
	void addScore();
	bool isDie();
	int getScore();

	sf::Vector2f getPos();
	sf::Vector2f getSize();

private:

	sf::Texture* texture;
	sf::RectangleShape	body;

	sf::Vector2f movment;
	sf::Vector2f windowSize;
	
	int score;
	bool _die;
	float deltaJump;
};

