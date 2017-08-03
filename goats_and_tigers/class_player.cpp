#define _CRT_SECURE_NO_WARNINGS
#include "goats_and_tigers.h"
#include "class_player.h"

void player::set_side(char side) {
	this->side = side;
}

char player::get_side(void) {
	return side;
}

void player::set_my_turn(bool my_turn) {
	this->my_turn = my_turn;
}

bool player::get_my_turn(void) {
	return my_turn;
}

void player::set_nickname(char* nickname) {
	strcpy(this->nickname, "komputer");
}

char* player::get_nickname(void) {
	return nickname;
}