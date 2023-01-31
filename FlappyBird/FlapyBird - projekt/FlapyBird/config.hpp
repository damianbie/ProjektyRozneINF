#pragma once

#include <conio.h>
#include <memory>
#include <string>
#include <iostream>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>


enum GameState
{
	Pause,
	Run,
	LaunchGame, 
	Lose,
	EndGame,
	NotStarted
};

class Game;
class Player;
class Obstacle;

#define FPS					200
#define TIMETOSTARTGAME		0
#define DELAY_AFTER_LOSE	1.0f
#define JUMP_Y				100

using namespace std;
using namespace sf;


#include "Player.h"
#include "Obstacle.h"
#include "Game.h"
#include "ScoreBoard.h"
