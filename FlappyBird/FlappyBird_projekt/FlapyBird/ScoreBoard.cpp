#pragma once
#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
	fstream file(this->SCOREBOARD_FILE, ios::binary | ios::in);
	if (!file.good())
	{
		file.close();
		cout << "Restoring score board..." << endl;
		this->saveDefault();
		cout << "Score board created" << endl;
		file.open(this->SCOREBOARD_FILE, ios::binary | ios::in);
	}

	int size;			// nickname size
	file.read((char*)&size, sizeof(size));

	char* txt = new char[size+1];
	txt[size] = '\0';
	file.read(txt, size);

	this->nickName = txt;
	delete[] txt;

	file.read((char*)&size, sizeof(size));	// 
	this->max_score = size;

	file.read((char*)&size, sizeof(size));
	txt = new char[size + 1];
	txt[size] = '\0';
	file.read(txt, size);

	this->max_nickname = txt;
	delete[] txt;



	file.read((char*)&size, sizeof(size));	// number of rows
	this->size = size;

	for (int i = 0; i < this->size; i++)
	{
		// nick size
		file.read((char*)&size, sizeof(size));

		//load nick
		txt = new char[size + 1];
		txt[size] = '\0';
		file.read(txt, size);
		this->names.push_back(string(txt));
		delete[] txt;

		//load score
		int sc;
		file.read((char*)&sc, sizeof(sc));
		this->scores.push_back(sc);

		file.read((char*)&sc, sizeof(sc));
		this->times.push_back(sc);
	}
}
ScoreBoard::~ScoreBoard()
{
	if (this->names.size() > 10)
	{
		// usuwamy stare wyniki 

		this->names.erase(this->names.begin(), this->names.end() - 10);
		this->scores.erase(this->scores.begin(), this->scores.end() - 10);
		this->times.erase(this->times.begin(), this->times.end() - 10);
		this->size = names.size();
	}

	fstream file(this->SCOREBOARD_FILE, ios::binary | ios::out);

	//save nickname
	int s = this->nickName.length();
	file.write((char*)&s, sizeof(s));
	file.write(this->nickName.c_str(), this->nickName.length());

	//najlepszy wynik 
	file.write((char*)&this->max_score, sizeof(this->max_score));
	s = this->max_nickname.length();
	// zapisanie pseudonimu
	file.write((char*)&s, sizeof(s));
	file.write(max_nickname.c_str(), max_nickname.length());

	// zapis liczby elementow 
	file.write((char*)&this->size, sizeof(this->size));

	string buff;
	int _nSize, score, time;
	for (int i = 0; i < this->size; i++)
	{
		// nick_length | nick | score | time [s]
		// 4           | ...  |   4   |   4

		//nick
		_nSize = this->names[i].length();
		file.write((char*)&_nSize, sizeof(_nSize));
		file.write(this->names[i].c_str(), this->names[i].length());
		//score
		file.write((char*)&this->scores[i], sizeof(this->scores[i]));
		//time 
		file.write((char*)&this->times[i], sizeof(this->times[i]));
	}
}
void ScoreBoard::saveDefault()
{
	fstream file(this->SCOREBOARD_FILE, ios::binary | ios::out);
	string buff;

	buff = "TwojPseudonim";
	int s = buff.length();
	// zapisanie pseudonimu
	file.write((char*)&s, sizeof(s));
	file.write(buff.c_str(), buff.length());

	// zapis najlepszego wyniku
	this->max_score = 0;
	file.write((char*)&this->max_score, sizeof(this->max_score));

	//pseudonim najlepszego gracza
	this->max_nickname = "Brak danch";
	s = this->max_nickname.length();
	// zapisanie pseudonimu
	file.write((char*)&s, sizeof(s));
	file.write(max_nickname.c_str(), max_nickname.length());

	s = 2;
	file.write((char*)&s, sizeof(s));

	for (int i = 0; i < s; i++)
	{
		// d³ugosc pseudonimu	| pseudonim | punkty | czas gry [s]
		//			4           | ...		|   4	 |   4

		buff = "Gracz_" + std::to_string(i+1);

		int _nSize = buff.length();
		file.write((char*)&_nSize, sizeof(_nSize));
		file.write(buff.c_str(), buff.length());
		
		int score = 10 + 4*i;
		file.write((char*)&score, sizeof(score));

		int time = 15 + 2*i;
		file.write((char*)&time, sizeof(time));
	}
}
void ScoreBoard::addNewScore(int score, int time)
{
	if (this->nickName.empty()) return;
	if (this->max_score < score)
	{
		this->max_score = score;
		this->max_nickname = this->nickName;
	}
	this->names.push_back(this->nickName);
	this->scores.push_back(score);
	this->times.push_back(time);
	this->size++;
}
void ScoreBoard::setNickName(string nc)
{
	this->nickName = nc;
}
void ScoreBoard::setNickName(sf::String nc)
{
	this->nickName.clear();
	for (auto it = nc.begin(); it != nc.end(); it++)
		this->nickName += *it;
}
int ScoreBoard::getTheBestScore() const
{
	return this->max_score;
}
sf::String ScoreBoard::getSavedNickName() const
{
	return sf::String(this->nickName);
}
std::string ScoreBoard::getTheBestNickName() const
{
	return this->max_nickname;
}
vector<std::string> ScoreBoard::getNickNames()
{
	return this->names;
}
vector<int> ScoreBoard::getScore()
{
	return this->scores;
}
vector<int> ScoreBoard::getTime()
{
	return this->times;
}