#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <random>			//default_random_engine
#include <algorithm>		//random_shuffle
#include <windows.h>		//Sleep
#include <time.h>			//srand

using namespace std;

const int number_of_decks = 8;		//liczba talii, których używamy

void title()
{
	system("cls");
	cout << "==================================================" << endl;
	cout << "                    BlackJack                     " << endl;
	cout << "==================================================" << endl << endl;
}

void rules()
{
	cout << "Rules: " << endl << endl;
	cout << "1) Casino is using "<< number_of_decks << " decks, each has 52 cards." << endl;
	cout << "2) The game has no tokens, nor any betting system." << endl;
	cout << "3) You only choose to hit or stand, other options" << endl;
	cout << "   will show up if available." << endl;
	cout << "4) Press q to quit anytime." << endl << endl;
	cout << "Good luck and have fun!" << endl << endl;
	cout << endl;
	system("pause");
	title();
}

string askForName()
{
	cout << "What's Your name?" << endl;
	string input;
	cin.clear();
	cin >> input;
	return input;
}

void Quit()
{
	system("cls");
	cout << "==================================================" << endl;
	cout << "		See You soon!" << endl;
	cout << "==================================================" << endl << endl;
	system("pause");
	exit(0);
}

bool Insurance()
{
	char decision;
	cout << "Do you want insurance?" << endl;
	cout << "[Y/N]: ";
	while (true) {
		cin.clear();
		cin >> decision;
		switch (decision) {
		case 'Y':
		case 'y':
			cout << "Insured!" << endl;
			return 1;
		case 'N':
		case 'n':
			cout << "As You wish!" << endl;
			return 0;
		case 'Q':
		case 'q':
			Quit();
		default:
			cout << "Unknown input, try again!" << endl;
			break;
		}
		cin.ignore(1000, '\n');
	}
}

bool DoubleDown()
{
	char decision;
	cout << "It is less than 11! Double down?" << endl;
	cout << "[Y/N]: ";
	while (true) {
		cin.clear();
		cin >> decision;
		switch (decision) {
		case 'Y':
		case 'y':
			cout << "Double down!" << endl;
			return 1;
		case 'N':
		case 'n':
			cout << "As You wish!" << endl;
			return 0;
		case 'Q':
		case 'q':
			Quit();
		default:
			cout << "Unknown input, try again!" << endl;
			break;
		}
	}
}

bool Hit()
{
	char decision;
	cout << endl;
	cout << "Do you want to hit? " << endl;
	cout << "[Y/N]: ";
	while (true) {
		cin.clear();
		cin >> decision;
		cin.ignore(1000, '\n');
		switch (decision) {
		case 'Y':
		case 'y':
			cout << "Here you go!" << endl;
			return 1;
		case 'N':
		case 'n':
			cout << "As You wish!" << endl << endl;
			return 0;
		case 'Q':
		case 'q':
			Quit();
		default:
			cout << "Unknown input, try again!" << endl;
			break;
		}
	}
}

class Card {
private:
	int rank;	//wartość karty na potrzeby gry
	int name;	//nr karty - 1-A, 2-10 normalnie 11-J, 12-Q, 13-K
	int suit;	//kolor 

public:
	Card(int n, int s);
	int getRank();
	void show();
};

class Shoe {
public:
	//STL josoutis obsługa vectora 
	vector<Card> deck;
	Shoe();
	void Shuffle();
	Card DrawCard();
};

class Player {
public:
	Player(string _name);
	Card upcard(Shoe & _shoe);
	Card downcard(Shoe & _shoe);
	Card hit(Shoe &_shoe);
	int getScore();
	void minus();
	void showScore();
	int getAces();
	string name;
	bool insured;
private:
	int aces;
	int score;
};


int main() {
	SetConsoleTitle(TEXT("BlackJack Game"));
	title();
	rules();
	string name = askForName();
	while (true)
	{
		srand((unsigned)time(NULL));
		bool gameover = 0;
		Shoe s;

		//utworzenie graczy
		Player p1("Dealer");
		Player p2(name);
		title();

		//rozdanie dla krupiera 
		Card upcard = p1.upcard(s);
		Card downcard = p1.downcard(s);

		//rozdanie dla gracza
		p2.hit(s);
		p2.hit(s);
		p2.showScore();
		cout << endl;
		bool dd = 0;	//double down
		if (p2.getScore() < 11)
		{
			if (DoubleDown()) dd = true;
		}

		//dobieranie
		while (!s.deck.empty())
		{
			if (p2.getScore() == 21)
			{
				cout << "Natural 21! What a luck!" << endl << endl;
				break;
			}
			if (dd || Hit())		//jeśli double down to nie pytaj czy hit
			{
				p2.hit(s);
				p2.showScore();
				if (p2.getScore() == 21)
				{
					cout << endl;
					cout << "BLACKJACK!!!" << endl << endl;
					break;
				}
				if (p2.getScore() > 21 && p2.getAces() > 0)
				{
					p2.minus();
				}

				if (p2.getScore() > 21)
				{
					cout << "You bust! Game Over!" << endl;
					gameover = true;
					break;
				}
				if (dd) break;		// jeśli double down to dobierz tylko jedną kartę
			}
			else break;
		}

		//dobieranie przez krupiera
		if (!gameover)
		{
			while (!s.deck.empty())
			{
				if (p1.getScore() < 17)		//krupier dobiera dopóki nie ma więcej niż 17
				{
					p1.hit(s);

					if (p2.getScore() > 21 && p2.getAces() > 0)
					{
						p2.minus();
					}

					else if (p2.getScore() > 21)
					{
						cout << "BUST!" << p2.name << " wins!" << endl;
						break;
					}
				}
				else break;
			}
			cout << "Dealers downcard (#####) was: ";
			downcard.show();
			if (upcard.getRank() == 1 && downcard.getRank() == 10)		//jeśli była opcja insurance i zadziałało
			{
				cout << endl;
				if (p2.insured) cout << "Taking the insurance was a perfect choice!";
				else cout << "Unlucky! You should have taken the insurance...";
			}
			cout << endl << endl << endl;
			p1.showScore();
			p2.showScore();
			cout << endl;

			if (p1.getScore() == p2.getScore()) cout << "That's a tie!" << endl;
			if (p2.getScore() > p1.getScore() || p1.getScore() > 21) cout << p2.name << " wins!" << endl;
			else if (p1.getScore() > p2.getScore()) cout << "Casino wins!" << endl;
		}
		cout << endl << endl;
		cout << "PLAY AGAIN?" << endl;
		cout << "[Y/N]" << endl;
		cin.clear();
		char decision;
		cin >> decision;
		if (decision == 'n' || decision == 'N')
		{
			Quit();
		}
		cin.ignore(1000, '\n');
	}
}

Shoe::Shoe()
{
	int suit[4] = { 1, 2, 3, 4 };		// 4 kolory
	int name[13] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };	//13 kart

	//stworz kartę i wrzuć do wektora
	for (int i = 0; i < number_of_decks; i++)
	{
		cout << endl;
		for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 13; j++) {
				deck.push_back(Card(j, i));
			}
		}
	}
	Shuffle();
}

void Shoe::Shuffle()
{
	title();
	cout << "Shuffling";
	for (int i = 0; i < 5; i++) {
		cout << ".";
		Sleep(250);
	}
	title();
	//TODO: ewentualnie zmienić funkcję random
	auto rng = std::default_random_engine{};
	random_shuffle(deck.begin(), deck.end());
}

Card Shoe::DrawCard()
{
	Card c = deck.back();
	deck.pop_back();
	return c;
}

Card::Card(int _name, int _suit)
{
	name = _name;
	suit = _suit;
	rank = getRank();
}

int Card::getRank()
{
	switch (name) {
	case 1:
		return 1;		//albo 11 - pamiętaj
	case 11:
	case 12:
	case 13:
		return 10;		//figury
	default:
		return name;	//karty 2-10
	}
}

void Card::show()
{
	switch (name) {
	case 1:
		cout << "Ace of ";
		break;
	case 11:
		cout << "Joker of ";
		break;
	case 12:
		cout << "Queen of ";
		break;
	case 13:
		cout << "King of ";
		break;
	default:
		cout << name << " of ";
		break;
	}
	switch (suit) {
	case 1:
		cout << "Clubs";
		break;
	case 2:
		cout << "Diamonds";
		break;
	case 3:
		cout << "Hearts";
		break;
	case 4:
		cout << "Spades";
		break;
	default:
		cout << "I HAVE NO IDEA WHAT" << endl;
		break;
	}
}

Player::Player(string _name)
{
	insured = false;
	score = 0;
	aces = 0;
	name = _name;
}

Card Player::upcard(Shoe &_shoe)
{
	Card x = _shoe.DrawCard();
	cout << name << "'s card: ";
	x.show();
	if (x.getRank() == 1)		//jeżeli pobrano asa
	{
		score += 11;
		aces++;
		cout << endl << endl;
		insured = Insurance();
		return x;
	}
	score += x.getRank();
	return x;
}

Card Player::downcard(Shoe &_shoe)
{
	Card x = _shoe.DrawCard();
	cout << endl << name << "'s downcard: ##########" << endl;
	cout << endl;
	if (x.getRank() == 1)	//jeżeli pobrano asa
	{
		if (score + 11 <= 21)	//jeśli możliwe As=11
		{
			score += 11;
			aces++;			//zwieksz liczbe asów wartych 11
			return x;
		}
	}
	score += x.getRank();
	return x;
}

Card Player::hit(Shoe &_shoe)
{
	Card x = _shoe.DrawCard();
	cout << name << "'s card: ";
	x.show();
	cout << endl;
	if (x.getRank() == 1)	//jeżeli pobrano asa
	{
		if (score + 11 <= 21)	//jeśli można As=11
		{
			score += 11;
			aces++;				//zwiększ liczbę asów wartych 11 
			return x;
		}
	}
	if (score + x.getRank() > 21 && aces > 0) minus();		//jeśli przebiłeś, as=1 zamiast 11;
	score += x.getRank();
	return x;
}

int Player::getScore()
{
	return score;
}

void Player::minus()
{
	score -= 10;
	aces--;
}

void Player::showScore()
{
	cout << name << "'s score : " << getScore() << endl;
}

int Player::getAces()
{
	return aces;
}
