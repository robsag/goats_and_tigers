#ifndef GOATS_AND_TIGERS_H
#define GOATS_AND_TIGERS_H

#include <iostream>
#include <cstdio>
#include <cstdlib> //system
#include <string> //string
#include <conio.h> //_getch
#include <windows.h> //kolory
#include <cctype> //isdigit
#include <cstring>
#include <time.h>

using namespace std;

struct game_info {
	char mode;
	char side;
	char nickname1[16];
	char nickname2[16];
	char whose_turn;
	char fields_values[25];
	int eaten_goats;
	int round;
};

struct score {
	char nickname[16];
	int wins;
	int losses;
	struct score* next;
	struct score* previous;
};

void gotoxy(int x, int y);
COORD wherexy(void);

#endif