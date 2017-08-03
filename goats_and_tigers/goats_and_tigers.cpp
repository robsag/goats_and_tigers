#define _CRT_SECURE_NO_WARNINGS //fopen, fscanf
#include "goats_and_tigers.h"
#include "menu.h"
#include "graphic_effects.h"
#include "sound_effects.h"
#include "class_board.h"
#include "class_player.h"
#include "class_human_player.h"
#include "class_computer_player.h"

void singleplayer(game_info* info);
void multiplayer(game_info* info);
void save_game(game_info* info, board* board, human_player* player1);
bool if_someone_wins(board* board, human_player* player1);
bool if_goats_wins(board* board, human_player* player1);
bool if_tigers_wins(board* board);
void stats_update(char* winner, char* loser);
void score_sort(score *head);
template <typename T1, typename T2, typename T3>
void array_sort(T1 *array1, T2 *array2, T3 array_length);
void count_blocked_tigers(board *board);
void gotoxy(int, int);
COORD wherexy();

//**********************************************************************************************************************************
//**********************************************************************************************************************************
int main() {
	check_game_files();
	console_settings();
	intro();

	while (true) {
		game_info info;
		bool if_is_save;

		int choice = menu();

		switch (choice) {
		case 1: new_game(&info);
			if (info.mode == 's')
				singleplayer(&info);
			else
				multiplayer(&info);
			break;
		case 2: 
			if_is_save = resume_game(&info);
			if (if_is_save == false)
				continue;
			if (info.mode == 's')
				singleplayer(&info);
			else
				multiplayer(&info);
			break;
		case 3: stats();
			continue; 
		case 4: how_to_play();
			continue;
		case 5: about();
			continue;
		case 6: end_game();
			return 0;
		}
	}
}
//**********************************************************************************************************************************
//**********************************************************************************************************************************

void singleplayer(game_info* info) {
	system("cls");
	board board;
	human_player player1;
	computer_player player2;

	player1.set_side(info->side);
	if (info->side == 'k')
		player2.set_side('t');
	else
		player2.set_side('k');

	player1.set_nickname(info->nickname1);
	player2.set_nickname(NULL); //zeby byl polimorfizm

	if (info->side == info->whose_turn) {
		player1.set_my_turn(true);
		player2.set_my_turn(false);
	}
	else {
		player1.set_my_turn(false);
		player2.set_my_turn(true);
	}

	for (int i = 0; i < 25; i++)
		board.set_field_value(i, info->fields_values[i]);

	board.set_eaten_goats(info->eaten_goats);

	COORD move = { 8,5 };

	bool end = false, save;
	while (true) {
		gotoxy(59, 5);
		cout << "Runda " << info->round;

		while (end != true) {
			draw_board(board.get_fields_values());

			gotoxy(59, 7);
			cout << "Zjedzone kozy: " << board.get_eaten_goats(); //nie trzeba czyscic linii komunikatu
			count_blocked_tigers(&board);
			gotoxy(59, 9);
			cout << "Zablokowane tygrysy: " << board.get_blocked_tigers(); //nie trzeba czyscic linii komunikatu

			if (if_someone_wins(&board, &player1) == true) { //wygra gracz ktory ostatni wykonal ruch
				char winner[16];
				char loser[16];
				if (player1.get_my_turn() == false) {
					strcpy(winner, player1.get_nickname());
					strcpy(loser, player2.get_nickname());
				}
				else {
					strcpy(winner, player2.get_nickname());
					strcpy(loser, player1.get_nickname());
				}

				winner_graphics(winner);
				stats_update(winner, loser);
				while (char end = _getch() != '\r');
				break;
			}

			if (player1.get_my_turn() == true) {
				bool escape = false;
				bool cancel_move = false;
				move = player1.move(move, end, save, &board, escape, cancel_move);
				if (end == true) {
					if (save == true)
						save_game(info, &board, &player1);
					break;
				}
				else if (escape == false && cancel_move == false) { // jezeli ruchem nie byl escape i jezeli nie anulowano zaznaczonego pionka
					player1.set_my_turn(false);
					player2.set_my_turn(true);
				}
			}
			else {
				move = player2.move(move, &board);
				player1.set_my_turn(true);
				player2.set_my_turn(false);
			}
		}

		info->round++;
		if (info->round > 2) //jezeli byly juz 2 rundy
			break;
		else { //przygotowanie drugiej rundy
			if (player1.get_side() == 'k') {
				player1.set_side('t');
				player2.set_side('k');
			}
			else {
				player1.set_side('k');
				player2.set_side('t');
			}

			info->side = player1.get_side();

			if (player1.get_side() == 'k') {
				player1.set_my_turn(true);
				player2.set_my_turn(false);
			}
			else {
				player1.set_my_turn(false);
				player2.set_my_turn(true);
			}

			info->whose_turn = 'k';

			for (int i = 0; i < 25; i++) {
				if (i == 0 || i == 4 || i == 20 || i == 24)
					board.set_field_value(i, 'T'); //tygrysy w rogach planszy
				else
					board.set_field_value(i, char(219));
			}

			board.set_eaten_goats(0);

			system("cls");
		}
	}
}

void multiplayer(game_info* info) {
	system("cls");
	board board;
	human_player player1;
	human_player player2;

	player1.set_side(info->side);
	if (info->side == 'k')
		player2.set_side('t');
	else
		player2.set_side('k');

	player1.set_nickname(info->nickname1);
	player2.set_nickname(info->nickname2);

	if (info->side == info->whose_turn) {
		player1.set_my_turn(true);
		player2.set_my_turn(false);
	}
	else {
		player1.set_my_turn(false);
		player2.set_my_turn(true);
	}

	for (int i = 0; i < 25; i++) 
		board.set_field_value(i, info->fields_values[i]);

	board.set_eaten_goats(info->eaten_goats);

	COORD move = { 8,5 };

	bool end = false, save;
	while (true) {
		gotoxy(59, 5);
		cout << "Runda " << info->round;

		while (end != true) {
			draw_board(board.get_fields_values());

			gotoxy(59, 9);
			cout << "Zjedzone kozy: " << board.get_eaten_goats(); //nie trzeba czyscic linii komunikatu
			count_blocked_tigers(&board);
			gotoxy(59, 11);
			cout << "Zablokowane tygrysy: " << board.get_blocked_tigers(); //nie trzeba czyscic linii komunikatu

			if (if_someone_wins(&board, &player1) == true) { //wygra gracz ktory ostatni wykonal ruch
				char winner[16];
				char loser[16];
				if (player1.get_my_turn() == false) {
					strcpy(winner, player1.get_nickname());
					strcpy(loser, player2.get_nickname());
				}
				else {
					strcpy(winner, player2.get_nickname());
					strcpy(loser, player1.get_nickname());
				}

				winner_graphics(winner);
				stats_update(winner, loser);
				while (char end = _getch() != '\r');
				break;
			}

			gotoxy(56, 7);
			cout << "                                            "; //czyszczenie wiersza komunikatu (czyj ruch)

			if (player1.get_my_turn() == true) {
				gotoxy(59, 7);
				cout << "Ruch gracza: " << player1.get_nickname();
				if (player1.get_side() == 'k')
					cout << " (kozy)";
				else
					cout << " (tygrysy)";
				bool escape = false;
				bool cancel_move = false;
				move = player1.move(move, end, save, &board, escape, cancel_move);
				if (end == true) {
					if (save == true)
						save_game(info, &board, &player1);
					break;
				}
				if (escape == false && cancel_move == false) { // jezeli ruchem nie byl escape i jezeli nie anulowano zaznaczonego pionka
					player1.set_my_turn(false);
					player2.set_my_turn(true);
				}
			}
			else {
				gotoxy(59, 7);
				cout << "Ruch gracza: " << player2.get_nickname();
				if (player2.get_side() == 'k')
					cout << " (kozy)";
				else
					cout << " (tygrysy)";
				bool escape = false;
				bool cancel_move = false;
				move = player2.move(move, end, save, &board, escape, cancel_move);
				if (end == true) {
					if (save == true)
						save_game(info, &board, &player1);
					break;
				}
				if (escape == false && cancel_move == false) { // jezeli ruchem nie byl escape i jezeli nie anulowano zaznaczonego pionka
					player1.set_my_turn(true);
					player2.set_my_turn(false);
				}
			}
		}

		info->round++;
		if (info->round > 2) //jezeli byly juz 2 rundy
			break;
		else { //przygotowanie drugiej rundy
			if (player1.get_side() == 'k') {
				player1.set_side('t');
				player2.set_side('k');
			}
			else {
				player1.set_side('k');
				player2.set_side('t');
			}

			info->side = player1.get_side();

			if (player1.get_side() == 'k') {
				player1.set_my_turn(true);
				player2.set_my_turn(false);
			}
			else {
				player1.set_my_turn(false);
				player2.set_my_turn(true);
			}

			info->whose_turn = 'k';

			for (int i = 0; i < 25; i++) {
				if (i == 0 || i == 4 || i == 20 || i == 24)
					board.set_field_value(i, 'T'); //tygrysy w rogach planszy
				else
					board.set_field_value(i, char(219));
			}

			board.set_eaten_goats(0);

			system("cls");
		}
	}
}

void save_game(game_info* info, board* board, human_player* player1) {
	system("attrib -H save.goats_and_tigers"); //usuniecie atrybutu ukrycia
	system("attrib -R save.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
	FILE *save;
	save = fopen("save.goats_and_tigers", "w");
	
	fprintf(save, "%c\n", info->mode);
	fprintf(save, "%c\n", info->side);
	fprintf(save, "%s\n", info->nickname1);
	fprintf(save, "%s\n", info->nickname2);
	if (player1->get_my_turn() == true)
		fprintf(save, "%c\n", player1->get_side());
	else if (player1->get_side() == 'k')
		fprintf(save, "%c\n", 't');
	else
		fprintf(save, "%c\n", 'k');
	char* tmp = board->get_fields_values();
	for (int i = 0; i < 25; i++)
		fprintf(save, "%c\n", tmp[i]);
	fprintf(save, "%d\n", board->get_eaten_goats());
	fprintf(save, "%d", info->round);

	fclose(save);
	system("attrib +R save.goats_and_tigers"); //ustawienie atrybutu tylko do odczytu
	system("attrib +H save.goats_and_tigers"); //ustawienie atrybutu ukrycia
}

bool if_someone_wins(board* board, human_player* player1) {
	if (if_goats_wins(board, player1) == true)
		return true;

	if (if_tigers_wins(board) == true)
		return true;

	return false; //jezeli nikt nie wygral
}

bool if_goats_wins(board* board, human_player* player1) {
	if (board->eaten_goats == 5)
		return true;

	int goats[20] = { -1 };
	int j = 0;
	for (int i = 0; i < 25; i++) {
		if (board->fields_values[i] == 'K') { //zapisanie indeksow pol z kozami
			goats[j] = i;
			j++;
		}
	}

	if (goats[0] == -1) //jezeli nie ma jeszcze zadnej kozy na planszy
		return false;

	for (int i = 0; i < j; i++) { //jezeli ktorys z pionkow ma ruch to zwroc false
		for (int j = 0; j < 8; j++) { //sprawdzenie sasiadow
			if (board->fields_neighbors[goats[i]][j] != -1 && board->fields_values[board->fields_neighbors[goats[i]][j]] == char(219))
				return false;
		}
	}

	if (player1->get_my_turn() == true) //zamiana zwyciezcow
		player1->set_my_turn(false);
	else
		player1->set_my_turn(true);

	return true;
}

bool if_tigers_wins(board* board) {
	int tigers[4];
	for (int i = 0, j = 0; i < 25; i++) {
		if (board->fields_values[i] == 'T') { //zapisanie indeksow pol z tygrysami
			tigers[j] = i;
			j++;
		}
	}
	for (int i = 0; i < 4; i++) { //jezeli ktorys z pionkow ma ruch to zwroc false
		for (int j = 0; j < 8; j++) { //sprawdzenie sasiadow
			if (board->fields_neighbors[tigers[i]][j] != -1 && board->fields_values[board->fields_neighbors[tigers[i]][j]] == char(219))
				return false;
		}
		for (int j = 0; j < 8; j++) { //sprawdzenie bicia
			if (board->fields_captures[tigers[i]][j] != -1 && board->fields_values[board->fields_captures[tigers[i]][j]] == char(219))
				return false;
		}
	}

	return true;
}

void stats_update(char* winner, char* loser) {
	score *head = NULL, *pointer;
	FILE* stats = fopen("stats.goats_and_tigers", "r");
	char nickname[16] = { '\0' };
	int wins, losses;

	while (fscanf(stats, "%s %d %d", nickname, &wins, &losses) == 3) { //pobranie listy graczy z pliku
		score *new_pointer = new score;
		strcpy(new_pointer->nickname, nickname);
		new_pointer->wins = wins;
		new_pointer->losses = losses;
		new_pointer->next = NULL;
		new_pointer->previous = NULL;

		if (head == NULL)
			head = new_pointer;
		else {
			pointer = head;
			while (pointer->next != NULL)
				pointer = pointer->next;
			pointer->next = new_pointer;
			new_pointer->previous = pointer;
		}
	}
	fclose(stats);

	if (head == NULL) { //jezeli nie ma jeszcze zadnych wynikow
		system("attrib -H stats.goats_and_tigers"); //usuniecie atrybutu ukrycia
		system("attrib -R stats.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
		stats = fopen("stats.goats_and_tigers", "w");
		fprintf(stats, "%s %d %d\n", winner, 1, 0);
		fprintf(stats, "%s %d %d\n", loser, 0, 1);
		fclose(stats);
		system("attrib +R stats.goats_and_tigers"); //ustawienie atrybutu tylko do odczytu
		system("attrib +H stats.goats_and_tigers"); //ustawienie atrybutu ukrycia
	}
	else {
		bool winner_on_the_list = false, loser_on_the_list = false;
		pointer = head;
		while (pointer != NULL) {
			if (strcmp(pointer->nickname, winner) == 0) {
				winner_on_the_list = true;
				pointer->wins++;
				break;
			}
			pointer = pointer->next;
		}
		pointer = head;
		while (pointer != NULL) {
			if (strcmp(pointer->nickname, loser) == 0) {
				loser_on_the_list = true;
				pointer->losses++;
				break;
			}
			pointer = pointer->next;
		}

		if (winner_on_the_list == false) { //jezeli nie znaleziono takiego gracza
			score *new_pointer = new score;
			strcpy(new_pointer->nickname, winner);
			new_pointer->wins = 1;
			new_pointer->losses = 0;
			new_pointer->next = NULL;
			new_pointer->previous = NULL;

			pointer = head; //dodanie na koniec listy
			while (pointer->next != NULL)
				pointer = pointer->next;
			pointer->next = new_pointer;
			new_pointer->previous = pointer;
		}
		if (loser_on_the_list == false) { //jezeli nie znaleziono takiego gracza
			score *new_pointer = new score;
			strcpy(new_pointer->nickname, loser);
			new_pointer->wins = 0;
			new_pointer->losses = 1;
			new_pointer->next = NULL;
			new_pointer->previous = NULL;
			
			pointer = head; //dodanie na koniec listy
			while (pointer->next != NULL)
				pointer = pointer->next;
			pointer->next = new_pointer;
			new_pointer->previous = pointer;
		}

		score_sort(head); //sortowanie wynikow

		system("attrib -H stats.goats_and_tigers"); //usuniecie atrybutu ukrycia
		system("attrib -R stats.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
		stats = fopen("stats.goats_and_tigers", "w");
		pointer = head;
		while (pointer != NULL) {
			fprintf(stats, "%s %d %d\n", pointer->nickname, pointer->wins, pointer->losses);
			pointer = pointer->next;
		}
		fclose(stats);
		system("attrib +R stats.goats_and_tigers"); //ustawienie atrybutu tylko do odczytu
		system("attrib +H stats.goats_and_tigers"); //ustawienie atrybutu ukrycia

		pointer = head;
		while (pointer != NULL) { //zwolnienie pamieci
			score *tmp;
			tmp = pointer->next;
			delete pointer;
			pointer = tmp;
		}
	}
}

void count_blocked_tigers(board *board) {
	int blocked_tigers = 0;

	for (int i = 0; i < 25; i++) {
		if (board->fields_values[i] == 'T') {
			bool if_tiger_blocked = true;
			for (int j = 0; j < 8; j++) {
				if (board->fields_neighbors[i][j] != -1 && board->fields_values[board->fields_neighbors[i][j]] == char(219)) {
					if_tiger_blocked = false;
					break;
				}
			}
			for (int j = 0; j < 8; j++) { //nie trzeba sprawdzac czy jest koza miedzy polami ale trzeba sprawdzic czy jest tygrys
				if (board->fields_captures[i][j] != -1 && board->fields_values[board->fields_captures[i][j]] == char(219)) {
					int target_field_index = board->fields_captures[i][j];
					COORD target_field = board->fields[target_field_index];
					COORD selection_field = board->fields[i];
					int trash; //potrzebne tylko zeby funkcja dzialala

					if (if_can_capture(selection_field, target_field, target_field_index, trash, board) == true) { //sprawdzenie czy pod biciem nie jest tygrys (tygrys nie moze zbic tygrysa i wtedy bedzie zablokowany)
						if_tiger_blocked = false;
						break;
					}
				}
			}
			if (if_tiger_blocked == true)
				blocked_tigers++;
		}
	}
	
	board->set_blocked_tigers(blocked_tigers);
}

void gotoxy(int x, int y) { //od 1
	COORD c;
	c.X = x - 1; //kolumna
	c.Y = y - 1; //wiersz
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

COORD wherexy(void) { //od 0
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition;
}

void score_sort(score *head) {
	int counter = 0;
	score *pointer = head;
	while (pointer != NULL) {
		counter++;
		pointer = pointer->next;
	}

	int **score_array = new int*[counter];
	for (int i = 0; i < counter; i++)
		score_array[i] = new int[2];

	pointer = head;
	for (int i = 0; i < counter; i++) {
		score_array[i][0] = pointer->wins;
		score_array[i][1] = pointer->losses;
		pointer = pointer->next;
	}

	char **nicknames_array = new char*[counter];
	for (int i = 0; i < counter; i++)
		nicknames_array[i] = new char[16];

	pointer = head;
	for (int i = 0; i < counter; i++) {
		strcpy(nicknames_array[i], pointer->nickname);
		pointer = pointer->next;
	}

	array_sort(score_array, nicknames_array, counter);

	pointer = head;
	for (int i = 0; i < counter; i++) {
		strcpy(pointer->nickname, nicknames_array[i]);
		pointer->wins = score_array[i][0];
		pointer->losses = score_array[i][1];

		pointer = pointer->next;
	}
}

template <typename T1, typename T2, typename T3>
void array_sort(T1 *array1, T2 *array2, T3 array_length) {
	for (int i = 0; i < array_length - 1; i++) {
		int max[2] = { array1[i][0], array1[i][1] };
		int max_index = i; //indeks najlepszego wyniku
		for (int j = i; j < array_length; j++) {
			if (array1[j][0] > max[0] || (array1[j][0] == max[0] && array1[j][1] < max[1])) {
				max[0] = array1[j][0];
				max[1] = array1[j][1];
				max_index = j;
			}
		}
		int tmp[2] = { array1[i][0], array1[i][1] };
		array1[i][0] = max[0];
		array1[i][1] = max[1];
		array1[max_index][0] = tmp[0];
		array1[max_index][1] = tmp[1];

		char tmp2[16];
		strcpy(tmp2, array2[i]);
		strcpy(array2[i], array2[max_index]);
		strcpy(array2[max_index], tmp2);
	}
}