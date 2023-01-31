#include <iostream>
#include <iomanip>
#include "config.hpp"

using namespace std;
using namespace sf;

int main()
{
	ScoreBoard board;

#ifndef  _DEBUG
	sf::RenderWindow* wnd = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Flappy bird | 163461", sf::Style::None);
#else
	sf::RenderWindow* wnd = new sf::RenderWindow(sf::VideoMode(1024, 720), "Flappy bird | DEBUG", sf::Style::Titlebar | sf::Style::Close);
#endif //  DEBUG
	
	std::shared_ptr<sf::RenderWindow> window(wnd);

	Font flapyFont, font;
	if (!flapyFont.loadFromFile("data/FlappyBirdy.ttf"))
	{
		_getch();
		exit(0);
	}
	if (!font.loadFromFile("data/Verdana.ttf"))
	{
		_getch();
		exit(0);
	}

	short menuType = 0;
	Text title;
	title.setString("Flappy  Bird");
	title.setFont(flapyFont);
	title.setColor(sf::Color::White);
	title.setCharacterSize(160);
	title.setPosition(window->getSize().x/2 - (title.getGlobalBounds().width/2), window->getSize().y/8);

	String nickNameString(board.getSavedNickName());
	String tmpNickNameString;
	String error;
	Text nickName;
	nickName.setColor(sf::Color::White);
	nickName.setPosition(100, 500);
	nickName.setCharacterSize(28);
	nickName.setFont(font);

	Event ev;
	while (window->isOpen())
	{
		while (window->pollEvent(ev))
		{
			switch (ev.type)
			{
			case Event::Closed:
				window->close();
				break;
			case Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::BackSpace)
				{
					if (tmpNickNameString.getSize() > 0)
						tmpNickNameString.erase(tmpNickNameString.getSize() - 1, 1);
				}
				break;
			case Event::KeyReleased:
				if (ev.key.code == Keyboard::Escape)
				{
					if (menuType == 0)
						window->close();
					else
					{
						menuType = 0;
					}
				}
				else if (ev.key.code == Keyboard::Space && menuType == 0)
				{
					Game game(window, sf::VideoMode{ (unsigned int)window->getSize().x, (unsigned int)window->getSize().y });
					game.run();
					auto _t = game.getScoreAndTime();

					board.addNewScore(_t.first, _t.second);
				}
				else if (ev.key.code == Keyboard::A && menuType == 0)
				{
					cout << "About" << endl;
					menuType = 1;
				}
				else if (ev.key.code == Keyboard::U && menuType == 0)
				{
					cout << "nickname" << endl;
					menuType = 2;
					tmpNickNameString = nickNameString;
				}
				else if (ev.key.code == Keyboard::S && menuType == 0)
				{
					menuType = 3;
				}
				else if (menuType == 1)
				{
					menuType = 0;
				}
				else if (menuType == 2)
				{
					if (ev.key.code == sf::Keyboard::Enter)
					{
						if (tmpNickNameString.getSize() > 3 && tmpNickNameString.getSize() < 19)
						{
							// save
							//scoreBoard current nick
							nickNameString = tmpNickNameString;
							board.setNickName(nickNameString);
							menuType = 0;
							error = "";
						}
						else
						{
							error = L"Twój pseudonim musi mieæ od 4 do 18 znków";
						}
					}
				}
				else if (menuType == 3)
				{
					menuType = 0;
				}
				break;
				case sf::Event::TextEntered:
				{
					if (menuType == 2 && (ev.key.code > 32 && ev.key.code < 127)) //0-9 
						tmpNickNameString += (char)ev.key.code;
				}
				break;
			}			
		}

		window->clear();
	
		title.setString("Flappy  Bird");
		title.setFont(flapyFont);
		title.setColor(sf::Color::White);
		title.setCharacterSize(160);
		title.setPosition(window->getSize().x / 2 - (title.getGlobalBounds().width / 2), window->getSize().y / 20);
		window->draw(title);

		if (menuType == 0)
		{
			title.setFont(flapyFont);

			title.setCharacterSize(82);
			title.setString("Spacja aby rozpoczac\n\rA informacjie o grze \n\rU zmien pseudonim\n\rS Tablica wynikow \n\rEsc aby wyjsc");
			title.setPosition(window->getSize().x / 2 - (title.getGlobalBounds().width / 2), (window->getSize().y / 3) + 50);
			window->draw(title);
		}
		else if (menuType == 1)
		{
			title.setFont(font);

			title.setCharacterSize(28);
			title.setString("O grze\n\rGra zostala stworzona przez Damiana Bieleckiego\n\rjako projekt zaliczeniowy z c++ \n\rPolitechnika Rzeszowska im. Ignacego Lukasiewicza WEiI\n\rDamian Bielecki 2EFZI P1 163461 202.01.20");
			title.setPosition( window->getSize().x/2 - title.getGlobalBounds().width/2, (window->getSize().y / 3) + 150);
			window->draw(title);
		}
		else if (menuType == 2)
		{
			title.setFont(font);
			title.setCharacterSize(28);

			int y = (window->getSize().y / 2);

			title.setString(L"Ustaw swój pseudonim:");
			title.setPosition((window->getSize().x/2) - title.getLocalBounds().width, y);
			window->draw(title);

			nickName.setString(tmpNickNameString);
			nickName.setPosition( title.getPosition().x, y + title.getGlobalBounds().height + 10);
			window->draw(nickName);

			title.setString(L"Naciœnij enter aby zapisaæ, esc aby wyjœæ");
			title.setPosition(title.getPosition().x, y + title.getGlobalBounds().height*2 + 20);
			window->draw(title);

			title.setString(error);
			title.setPosition(title.getPosition().x, y + title.getGlobalBounds().height * 3 + 30);
			window->draw(title);
		}
		else if (menuType == 3)
		{
			int x = window->getSize().x / 2 - 400 / 2;
			int y = (window->getSize().y / 2);

			title.setFont(font);
			title.setCharacterSize(24);

			auto names = board.getNickNames();
			auto scores = board.getScore();
			auto times = board.getTime();
			// 30 4 4		
			reverse(names.begin(), names.end());
			reverse(scores.begin(), scores.end());
			reverse(times.begin(), times.end());

			int _yy = 30;


			title.setString("NAJLEPSZY WYNIK " + board.getTheBestNickName() + " ZDOBYL: "+ std::to_string(board.getTheBestScore()) + "PKT");
			title.setPosition(window->getSize().x/2 - title.getGlobalBounds().width/2, y - 2*_yy);
			window->draw(title);

			title.setString("Pseudonim");
			title.setPosition(x, y - _yy);
			window->draw(title);

			title.setString("Punkty");
			title.setPosition(x+250, y - _yy);
			window->draw(title);

			title.setString("Czas gry");
			title.setPosition(x+350, y - _yy);
			window->draw(title);

			for (int i = 0; i < 10 && i < names.size(); i++)
			{
				
				title.setString(names[i]);
				title.setPosition(x, y + _yy * i);
				window->draw(title);

				title.setString(std::to_string(scores[i]));
				title.setPosition(x + 250, y + _yy * i);
				window->draw(title);

				title.setString(std::to_string(times[i]) + "s");
				title.setPosition(x + 350, y + _yy * i);
				window->draw(title);
			}

		}
		window->display();
	}
	return 0;
}
