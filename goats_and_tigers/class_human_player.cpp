#define _CRT_SECURE_NO_WARNINGS
#include "goats_and_tigers.h"
#include "menu.h"
#include "class_board.h"
#include "class_player.h"
#include "class_human_player.h"
#include "graphic_effects.h"
#include "sound_effects.h"

COORD select_field(bool& end, bool& save, board* board, char side, bool& escape);
bool if_available_move(COORD point, board* board);
bool check_if_neighbor(COORD current_position, int field_index, board* board);
bool check_if_capture(COORD current_position, int field_index, board* board);
bool if_can_capture(COORD selection_field, COORD target_field, int target_field_index, int& capture_field_index, board* board);

void human_player::set_nickname(char* nickname) {
	strcpy(this->nickname, nickname);
}

COORD human_player::move(COORD before_position, bool& end, bool& save, board* board, bool& escape, bool& cancel_move) {
	gotoxy(before_position.X, before_position.Y);
	COORD selection_point = { 0,0 };

	int goats = 0;
	if (side == 'k') { //dla koz
		for (int i = 0; i < 25; i++) {
			if (board->fields_values[i] == 'K')
				goats++;
		}

		if (goats + board->eaten_goats == 20)
			selection_point = select_field(end, save, board, side, escape);
	}
	else //dla tygrysow
		selection_point = select_field(end, save, board, side, escape);

	if (escape == true)
		return selection_point;

	gotoxy(selection_point.X, selection_point.Y);

	COORD point;
	while (true)
	{
		point = wherexy();
		point.X++;
		point.Y++;
		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		gotoxy(56, 23);
		cout << "                                            "; //czyszczenie wiersza komunikatow
		gotoxy(point.X, point.Y);
		click_sound();

		switch (move)
		{
		case 72: //góra
			if (point.Y != 5) {
				gotoxy(point.X, point.Y - 9);
			}
			break;

		case 80: //do³
			if (point.Y != 41)
				gotoxy(point.X, point.Y + 9);
			break;

		case 77: //prawo
			if (point.X != 52)
				gotoxy(point.X + 11, point.Y);
			break;

		case 75: //lewo
			if (point.X != 8)
				gotoxy(point.X - 11, point.Y);
			break;

		case '\r':
		case '\n': { //bo dla windowsa enter to \r\n
			if (point.X == selection_point.X && point.Y == selection_point.Y) { //anulowanie wybranego pola
				cancel_move = true;
				return selection_point;
			}

			int i;
			for (i = 0; i < 25; i++) {
				if (board->fields[i].X == point.X && board->fields[i].Y == point.Y)
					break;
			}
			if (board->fields_values[i] != char(219)) { //jezeli pole jest zajete
				gotoxy(58, 23);
				cout << "Nie mozna postawic pionka w tym miejscu.";
				gotoxy(point.X, point.Y);
				break;
			}
			else if ((goats + board->eaten_goats == 20 || side == 't') && check_if_neighbor(selection_point, i, board) == false) { //check_if_neighbor sie nie wykona jesli wczesniejsza alternatywa bedzie falszywa
				int capture_field_index;
				if (check_if_capture(selection_point, i, board) == false) { //jezeli wybrane pole nie bylo sasiadem ani biciem
					gotoxy(58, 23);
					cout << "Nie mozna postawic pionka w tym miejscu.";
					gotoxy(point.X, point.Y);
					break;
				}
				else if (if_can_capture(selection_point, point, i, capture_field_index, board) == false) { //jezeli bylo biciem sprawdz czy mozna zbic
					gotoxy(58, 23);
					cout << "Nie mozna postawic pionka w tym miejscu.";
					gotoxy(point.X, point.Y);
					break;
				}
				else { //jezeli mozna zbic
					board->set_field_value(capture_field_index, char(219)); //usun koze z planszy
					board->eaten_goats++;
				}
			}

			board->set_field_value(i, toupper(get_side())); //postawienie pionka

			for (int i = 0; i < 25; i++) { //usuniecie przesunietego pionka
				if (board->fields[i].X == selection_point.X && board->fields[i].Y == selection_point.Y) {
					board->set_field_value(i, char(219));
					break;
				}
			}
			return point; }

		case 27: //escape
			escape = true;
			escape_key(end, save);
			return point;
		}
	}
}

COORD select_field(bool& end, bool& save, board* board, char side, bool& escape) {
	COORD point;
	while (true)
	{
		point = wherexy();
		point.X++;
		point.Y++;
		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		gotoxy(56, 23);
		cout << "                                            "; //czyszczenie wiersza komunikatow
		gotoxy(point.X, point.Y);
		click_sound();

		switch (move)
		{
		case 72: //góra
			if (point.Y != 5) {
				gotoxy(point.X, point.Y - 9);
			}
			break;

		case 80: //do³
			if (point.Y != 41)
				gotoxy(point.X, point.Y + 9);
			break;

		case 77: //prawo
			if (point.X != 52)
				gotoxy(point.X + 11, point.Y);
			break;

		case 75: //lewo
			if (point.X != 8 - 1)
				gotoxy(point.X - 11, point.Y);
			break;

		case '\r':
		case '\n': { //bo dla windowsa enter to \r\n
			int i;
			for (i = 0; i < 25; i++) {
				if (board->fields[i].X == point.X && board->fields[i].Y == point.Y)
					break;
			}
			if (board->fields_values[i] != toupper(side)) //jezeli na polu nie stoi pionek gracza ktory sie rusza
				break;
			if (if_available_move(point, board) == false) { //jezeli nieprawidlowy ruch (wybor pionka do przesuniecia)
				gotoxy(61, 23);
				cout << "Nie mozna ruszyc sie tym pionkiem.";
				gotoxy(point.X, point.Y);
				break;
			}
			select_field_graphics(point, side, 0xC9);
			return point; }

		case 27: //escape
			escape = true;
			escape_key(end, save);
			return point;
		}
	}
}

bool if_available_move(COORD point, board* board) {
	int i;
	for (i = 0; i < 25; i++) { //znalezienie indeksu pionka
		if (board->fields[i].X == point.X && board->fields[i].Y == point.Y)
			break;
	}
	for (int j = 0; j < 8; j++) {
		if (board->fields_neighbors[i][j] != -1 && board->fields_values[board->fields_neighbors[i][j]] == char(219)) //jezeli ktorys z sasiadow jest pusty
			return true;
		if (board->fields_captures[i][j] != -1 && board->fields_values[board->fields_captures[i][j]] == char(219)) //jezeli ktores z pol do bicia jest puste
			return true;
	}
	return false;
}

bool check_if_neighbor(COORD current_position, int field_index, board* board) {
	int previous_field_index;
	for (int i = 0; i < 25; i++) {
		if (board->fields[i].X == current_position.X && board->fields[i].Y == current_position.Y) {
			previous_field_index = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++) {
		if (board->fields_neighbors[previous_field_index][i] == field_index)
			return true;
	}
	return false;
}

bool check_if_capture(COORD current_position, int field_index, board* board) {
	int previous_field_index;
	for (int i = 0; i < 25; i++) {
		if (board->fields[i].X == current_position.X && board->fields[i].Y == current_position.Y) {
			previous_field_index = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++) {
		if (board->fields_captures[previous_field_index][i] == field_index)
			return true;
	}
	return false;
}

bool if_can_capture(COORD selection_field, COORD target_field, int target_field_index, int& capture_field_index, board* board) {
	COORD capture_field;
	if (selection_field.X < target_field.X)
		capture_field.X = selection_field.X + (target_field.X - selection_field.X) / 2;
	else
		capture_field.X = selection_field.X - (selection_field.X - target_field.X) / 2;

	if (selection_field.Y < target_field.Y)
		capture_field.Y = selection_field.Y + (target_field.Y - selection_field.Y) / 2;
	else
		capture_field.Y = selection_field.Y - (selection_field.Y - target_field.Y) / 2;

	for (int i = 0; i < 25; i++) {
		if (board->fields[i].X == capture_field.X && board->fields[i].Y == capture_field.Y) {
			capture_field_index = i;
			break;
		}
	}

	if (board->fields_values[capture_field_index] != 'K') //nie mozna zbic pustego pola ani tygrysa
		return false;
	else
		return true;
}
