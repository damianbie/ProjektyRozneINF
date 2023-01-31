#include "config.hpp"
#include "InputBox.h"

using namespace std;


InputBox::InputBox(int _x, int _y, int _width, int _height, std::shared_ptr<sf::Text> t)
	:pos(_x, _y), size(_height, _width), text(t)
{}
InputBox::InputBox(sf::Vector2i _pos, sf::Vector2i _size, std::shared_ptr<sf::Text> txt)
	:pos(_pos), size(_size), text(txt)
{}
void InputBox::draw(shared_ptr<sf::RenderWindow> _wnd)
{


}
void InputBox::update(sf::Event _ev)
{
	if (_ev.type == sf::Event::KeyReleased && this->isFocussed())
	{
		if (_ev.key.code == sf::Keyboard::Enter)
		{
			// save
		}
		else if(_ev.key.code == sf::Keyboard::BackSpace)
		{
			
		}
		else if (_ev.key.code > 127)
		{
			this->addCharacter(_ev.key.code);
		}
	}
}
void InputBox::removeOneCharacterFromString()
{
	auto str =  this->text->getString();
	
}
void InputBox::addCharacter(char ch)
{
	this->text->setString(this->text->getString() + ch);
}
sf::String InputBox::getText() const
{
	return this->text->getString();
}
int InputBox::getLength() const
{
	return this->text->getString().getSize();
}
bool InputBox::isFocussed() const
{
	return true;
}