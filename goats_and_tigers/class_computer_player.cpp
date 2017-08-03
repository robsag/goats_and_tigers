#define _CRT_SECURE_NO_WARNINGS
#include "goats_and_tigers.h"
#include "class_board.h"
#include "class_player.h"
#include "class_human_player.h"
#include "class_computer_player.h"
#include "sound_effects.h"

COORD put_a_goat(board* board);
COORD move_a_goat(board* board);
COORD move_a_tiger(board* board);

COORD computer_player::move(COORD before_position, board* board) {
	gotoxy(before_position.X, before_position.Y);
	COORD point;

	if (side == 'k') {
		int goats = 0;
		for (int i = 0; i < 25; i++) {
			if (board->fields_values[i] == 'K')
				goats++;
		}
		if (goats + board->eaten_goats < 20) //jezeli nie postawil jeszcze wszystkich pionkow
			point = put_a_goat(board);
		else
			point = move_a_goat(board);
	}
	else
		point = move_a_tiger(board);

	Sleep(250);
	click_sound();

	return point;
}

COORD put_a_goat(board* board) {
	int free_fields[25 - 4]; //bo sa 4 tygrysy
	int i, j;
	for (i = 0, j = 0; i < 25; i++) {
		if (board->fields_values[i] == char(219)) {
			free_fields[j] = i;
			j++;
		}
	}
	srand(time(NULL));
	int random_field = free_fields[rand() % j]; // 0 <= random_index <= j - 1, j - liczba wolnych pol, ale indeksy od 0 wiec do j - 1
	
	COORD move = board->fields[random_field];

	board->set_field_value(random_field, 'K'); //postawienie pionka

	return move;
}

COORD move_a_goat(board* board) {
	int free_goats[20]; //kozy ktore moga sie ruszyc
	int j = 0;
	for (int i = 0; i < 25; i++) {
		if (board->fields_values[i] == 'K') { //jezeli na polu stoi koza
			for (int k = 0; k < 8; k++) { //sprawdzenie sasiadow - czy ktorys jest pusty (czy mozliwy bedzie ruch)
				if (board->fields_neighbors[i][k] != -1 && board->fields_values[board->fields_neighbors[i][k]] == char(219)) {
					free_goats[j] = i;
					j++;
					break;
				}
			}
		}
	}
	srand(time(NULL));
	int random_goat = free_goats[rand() % j]; // 0 <= random_index <= j - 1, j - liczba wolnych koz, ale indeksy od 0 wiec do j - 1
	
	int free_neighbors[5]; //bo max 4 kozy moga byc zjedzone, a 1 pole jest zawsze wolne
	int k = 0;
	for (int l = 0; l < 8; l++) {
		if (board->fields_values[board->fields_neighbors[random_goat][l]] == char(219)) { //jezeli sasiad wylosowanej kozy jest pusty
			free_neighbors[k] = board->fields_neighbors[random_goat][l];
			k++;
		}
	}

	int random_field = free_neighbors[rand() % k]; // 0 <= random_index <= k - 1, k - liczba wolnych pol, ale indeksy od 0 wiec do k - 1
	
	COORD move = board->fields[random_field];

	board->set_field_value(random_goat, char(219)); //usuniecie przesuwanego pionka
	board->set_field_value(random_field, 'K'); //postawienie przesunietego pionka

	return move;
}

COORD move_a_tiger(board* board) {
	int free_captures[25 - 4][3];
	free_captures[0][0] = -1; //nie moze byc NULL bo NULL to zero
	int k = 0;
	for (int i = 0; i < 25; i++) {
		if (board->fields_values[i] == 'T') { //jezeli na polu stoi tygrys
			for (int j = 0; j < 8; j++) { //sprawdzenie czy jest bicie na wszystkich 8 mozliwych polach
				if (board->fields_captures[i][j] == -1) //jezeli koniec pol capture_fields
					break;
				
				if (board->fields_values[board->fields_captures[i][j]] != char(219)) //jezeli target field nie jest wolne
					continue;
				
				int target_field_index = board->fields_captures[i][j];
				COORD target_field = board->fields[target_field_index];
				COORD selection_field = board->fields[i];
				int capture_field_index;

				if (if_can_capture(selection_field, target_field, target_field_index, capture_field_index, board) == true) { //jezeli mozliwe bicie to dodaj do tablicy
					free_captures[k][0] = i; //selection field index
					free_captures[k][1] = capture_field_index; //capture field index
					free_captures[k][2] = target_field_index; //target field index
					k++;
				}
			}
		}
	}
	
	if (free_captures[0][0] != -1) { //jezeli jest bicie to bij
		int random_capture = rand() % k;
		
		int selection_field_index = free_captures[random_capture][0];
		int capture_field_index = free_captures[random_capture][1];
		int target_field_index = free_captures[random_capture][2];
		 
		COORD move = board->fields[free_captures[random_capture][2]];

		board->set_field_value(selection_field_index, char(219)); //usuniecie przesuwanego tygrysa
		board->set_field_value(capture_field_index, char(219)); //usuniecie zjedzonej kozy z planszy
		board->set_field_value(target_field_index, 'T'); //postawienie przesunietego tygrysa
		board->eaten_goats++;

		return move;
	}
	else { //jezeli nie ma bicia to rusz sie tygrysem na wolne pole
		int free_tigers[4]; //tygrysy ktore moga sie ruszyc
		int j = 0;
		for (int i = 0; i < 25; i++) {
			if (board->fields_values[i] == 'T') { //jezeli na polu stoi tygrys
				for (int k = 0; k < 8; k++) { //sprawdzenie sasiadow - czy ktorys jest pusty (czy mozliwy bedzie ruch)
					if (board->fields_neighbors[i][k] != -1 && board->fields_values[board->fields_neighbors[i][k]] == char(219)) {
						free_tigers[j] = i;
						j++;
						break;
					}
				}
			}
		}
		srand(time(NULL));
		int random_goat = free_tigers[rand() % j]; // 0 <= random_index <= j - 1, j - liczba wolnych tygrysow, ale indeksy od 0 wiec do j - 1

		int free_neighbors[8];
		int k = 0;
		for (int l = 0; l < 8; l++) {
			if (board->fields_values[board->fields_neighbors[random_goat][l]] == char(219)) { //jezeli sasiad wylosowanego tygrysa jest pusty
				free_neighbors[k] = board->fields_neighbors[random_goat][l];
				k++;
			}
		}

		int random_field = free_neighbors[rand() % k]; // 0 <= random_index <= k - 1, k - liczba wolnych pol, ale indeksy od 0 wiec do k - 1

		COORD move = board->fields[random_field];

		board->set_field_value(random_goat, char(219)); //usuniecie przesuwanego pionka
		board->set_field_value(random_field, 'T'); //postawienie przesunietego pionka

		return move;
	}
}