#include "config.hpp"
#include "Player.h"

Player::Player(sf::Vector2f size, std::string text, Vector2f winsize)
{
	texture = new Texture;
	if (!this->texture->loadFromFile(text))
	{
		_getch();
		exit(0);
	}
	this->body.setTexture(texture);
	this->body.setSize(size);

	int x = winsize.x / 2;
	int y = winsize.y / 2;

	this->windowSize = winsize;
	this->body.setPosition(x, y);

	deltaJump	= 0;
	score		= 0;
	_die		= false;
}
Player::~Player()
{
	delete texture;
}
void Player::draw(std::shared_ptr<RenderWindow> w)
{
	w->draw(this->body);
}
void Player::update(sf::Time dt, float newX)
{
	int y = body.getPosition().y;
	
	if (deltaJump <= 0)
	{
		y += (dt.asSeconds() * Game::gravicity.y);
	}
	else
	{
		float d = (dt.asSeconds() * deltaJump * 6);
		deltaJump -= d;
		if (d <= 0.40f)
			deltaJump = 0;
		y -= d;
		if (y < 0)
			y = 0;
	}

	body.setPosition(newX, y);
}
void Player::jump()
{
	deltaJump = JUMP_Y;
}
void Player::addScore()
{
	score++;
}
int Player::getScore()
{
	return score;
}
bool Player::isDie()
{
	if (body.getPosition().y > windowSize.y || _die)
		return true;
	else
		return false;
}
void Player::die()
{
	_die = true;
}
sf::Vector2f Player::getPos()
{
	return body.getPosition();
}
sf::Vector2f Player::getSize()
{
	return body.getSize();
}