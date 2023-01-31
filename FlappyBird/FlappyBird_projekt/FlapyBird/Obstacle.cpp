#include "Obstacle.h"


unsigned int Obstacle::BODY_WIDTH	= 160;
unsigned int Obstacle::HEAD_HEIGHT	= 60;
unsigned int Obstacle::HEAD_WIDTH	= 200;
unsigned int Obstacle::MULTIPLER	= 1;
Obstacle::Obstacle(float sX, float sY, float m, sf::Texture* headd, sf::Texture* bodyy)
	:posX(sX), startY(sY), margin(m), _life(true), _isScored(false)
{
	if (headd == nullptr)
	{
		cerr << "Failed to create obstacle" << endl
			<< "Bad texture!!";
		_getch();
		exit(0);
	}
	if (bodyy == nullptr)
	{
		cerr << "Failed to create obstacle" << endl
			<< "Bad texture!!";
		_getch();
		exit(0);
	}


	body.setTexture(bodyy);
	body.setPosition({posX, 0});
	body.setSize({ (float)Obstacle::BODY_WIDTH, startY});

	head.setTexture(headd);
	head.setPosition(sf::Vector2f{ (posX + (Obstacle::BODY_WIDTH/2)) - (Obstacle::HEAD_WIDTH/2), startY });
	head.setSize({ (float)Obstacle::HEAD_WIDTH, (float)Obstacle::HEAD_HEIGHT});
}
void Obstacle::update(sf::Time dt)
{
	if (this->posX + this->body.getGlobalBounds().width <= 0)
	{
		die();
	}

	float _x = (Game::speed * dt.asSeconds() * 1.60f);
	this->posX -= _x;

	this->body.setPosition(posX, body.getPosition().y);
	this->head.setPosition(sf::Vector2f{ (posX + (Obstacle::BODY_WIDTH / 2)) - (Obstacle::HEAD_WIDTH / 2), startY });
}
void Obstacle::score()
{
	_isScored = true;
}
void Obstacle::draw(std::shared_ptr<sf::RenderWindow> wnd)
{
	if (isOkay())
	{
		wnd->draw(this->head);
		wnd->draw(this->body);

		sf::Vector2f _bodyPos = body.getPosition();
		sf::Vector2f _bodySize = body.getSize();

		sf::Vector2f _headPos = head.getPosition();
		sf::Vector2f _headSize = head.getSize();


		head.move(0, this->margin);
		body.move(0, _bodySize.y + margin + Obstacle::HEAD_HEIGHT);
		body.setSize(sf::Vector2f(_bodySize.x, wnd->getSize().y - body.getPosition().y));

		wnd->draw(this->head);
		wnd->draw(this->body);


		head.setSize(_headSize);
		head.setPosition(_headPos);
		body.setSize(_bodySize);
		body.setPosition(_bodyPos);
	}
}
void Obstacle::die()
{
	this->_life = false;
}
bool Obstacle::isOkay()
{
	return this->_life;
}
bool Obstacle::collision(int _x, int _y, int _width, int _height)
{
	Vector2f pos = head.getPosition();
	Vector2f siz = head.getSize();
	if (_x > pos.x && _x + _width <= pos.x + siz.x || _x + _width > pos.x && _x + _width <= pos.x + siz.x)
	{
		if (_y <= pos.y + siz.y)
			return true;
		else if (_y + _height >= pos.y + margin)
			return true;
	}
	return false;
}

bool Obstacle::isScored()
{
	return _isScored;
}

sf::Vector2f Obstacle::getBodyPos()
{
	return body.getPosition();
}

sf::Vector2f Obstacle::getBodySize()
{
	return body.getSize();
}
