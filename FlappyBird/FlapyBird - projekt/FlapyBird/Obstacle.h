#pragma once
#include "config.hpp"

class Obstacle
{
public:
	Obstacle(float posX, float startY, float margin, sf::Texture* head, sf::Texture* body);

	void draw(std::shared_ptr<sf::RenderWindow> wnd);
	void update(sf::Time dt);
	void score();

	bool isOkay();
	bool collision(int x, int y, int width, int height);
	bool isScored();
	
	sf::Vector2f getBodyPos();
	sf::Vector2f getBodySize();

	static unsigned int BODY_WIDTH;
	static unsigned int HEAD_WIDTH;
	static unsigned int HEAD_HEIGHT;
	static unsigned int MULTIPLER;
private:
	void die();

	bool				_isScored;
	bool				_life;
	float				posX;
	float				startY;
	float				margin;

	sf::RectangleShape	head;
	sf::RectangleShape	body;
};

