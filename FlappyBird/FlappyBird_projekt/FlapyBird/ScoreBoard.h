#pragma once
#include <string>
#include <fstream>
#include "config.hpp"

class ScoreBoard
{
public: 
	ScoreBoard();
	~ScoreBoard();

	void addNewScore(int score, int time);
	void setNickName(string name);
	void setNickName(sf::String name);
	
	int getTheBestScore() const;

	sf::String getSavedNickName() const;
	std::string getTheBestNickName() const;


	std::vector<string> getNickNames();
	std::vector<int> getTime();
	std::vector<int> getScore();

	const string SCOREBOARD_FILE = "score.board";
private:
	void saveDefault();

	int size;
	int max_score;
	string max_nickname;
	std::vector<string> names;
	std::vector<int> scores;
	std::vector<int> times;

	string nickName;

};