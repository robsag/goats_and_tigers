#define _CRT_SECURE_NO_WARNINGS
#include "goats_and_tigers.h"
#include "graphic_effects.h"
#include "sound_effects.h"

char pick_a_mode(void);
char pick_a_side(char);
void set_nicknames(char*, char*, char);
void get_nickname(char*);
bool if_end(void);
bool if_save(void);

void check_game_files(void) {
	FILE *stats;
	if ((stats = fopen("stats.goats_and_tigers", "r")) == NULL)
		stats = fopen("stats.goats_and_tigers", "w");
	fclose(stats);
	system("attrib +R stats.goats_and_tigers"); //ustawienie atrybutu tylko do odczytu
	system("attrib +H stats.goats_and_tigers"); //ustawienie atrybutu ukrycia
}

void intro(void) {
	for (int i = 0; i < 4; i++)
	{
		system("cls");
		Sleep(600); //0,6 sek
		intro_graphics();
		Sleep(700); //0,7 sek
	}

	gotoxy(55, 44);
	change_color(28);
	cout << "nacisnij dowolny klawisz aby kontynuowac...";
	change_color(26);
	_getch();
	system("cls");
}

int menu() {
	system("cls");
	menu_graphics();
	//int sound_start = 0; //ustawienie na pocz¹tek melodii

	int choice = 1;
	while (true) {
		gotoxy(100, 45);
		//sound_start = menu_sound(sound_start) + 1; //muzyka

		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		new_game_graphics();
		resume_game_graphics();
		stats_graphics();
		how_to_play_graphics();
		about_graphics();
		end_game_graphics();

		switch (move) {
		case 72: //góra
			if (choice != 1) {
				choice--;
				switch (choice) {
				case 1: new_game_graphics(28); break;
				case 2: resume_game_graphics(28); break;
				case 3: stats_graphics(28); break;
				case 4: how_to_play_graphics(28); break;
				case 5: about_graphics(28); break;
				}
			}
			else {
				choice = 6;
				end_game_graphics(28);
			}
			break;

		case 80: //do³
			if (choice != 6) {
				choice++;
				switch (choice) {
				case 2: resume_game_graphics(28); break;
				case 3: stats_graphics(28); break;
				case 4: how_to_play_graphics(28); break;
				case 5: about_graphics(28); break;
				case 6: end_game_graphics(28); break;
				}
			}
			else {
				choice = 1;
				new_game_graphics(28);
			}
			break;

		case '\r':
		case '\n': //bo dla windowsa enter to \r\n;
			return choice;
			break;

		default:
			switch (choice) {
			case 1: new_game_graphics(28); break;
			case 2: resume_game_graphics(28); break;
			case 3: stats_graphics(28); break;
			case 4: how_to_play_graphics(28); break;
			case 5: about_graphics(28); break;
			case 6: end_game_graphics(28); break;
			}
			break;
		}

		//click_sound(); //TO DO
	}
}

void new_game(game_info* info) {
	char mode = pick_a_mode(); //single lub multi
	char side = pick_a_side(mode); //kozy lub tygrysy
	char nickname1[16] = { '\0' }, nickname2[16] = { '\0' };
	set_nicknames(nickname1, nickname2, mode);

	info->mode = mode;
	info->side = side;
	strcpy(info->nickname1, nickname1);
	strcpy(info->nickname2, nickname2);
	info->whose_turn = 'k'; //bo kozy zaczynaja
	for (int i = 0; i < 25; i++){
		if (i == 0 || i == 4 || i == 20 || i == 24)
			info->fields_values[i] = 'T'; //tygrysy w rogach planszy
		else
			info->fields_values[i] = char(219);
	}
	info->eaten_goats = 0;
	info->round = 1;
}

char pick_a_mode(void) {
	system("cls");
	pick_a_mode_graphics();
	singleplayer_mode_graphics(28);
	multiplayer_mode_graphics();

	char choice = 's';
	while (true) {
		gotoxy(100, 45);

		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		singleplayer_mode_graphics();
		multiplayer_mode_graphics();

		switch (move) {
		case 72: //góra
		case 80: //do³
			if (choice == 's') {
				choice = 'm';
				multiplayer_mode_graphics(28);
			}
			else {
				choice = 's';
				singleplayer_mode_graphics(28);
			}
			break;

		case '\r':
		case '\n': //bo dla windowsa enter to \r\n
			return choice;
			break;

		default:
			switch (choice) {
			case 's': singleplayer_mode_graphics(28); break;
			case 'm': multiplayer_mode_graphics(28); break;
			}
			break;
		}
	}
}

char pick_a_side(char choice) {
	system("cls");
	if (choice == 's') { //singleplayer
		pick_a_side_graphics();
		goats_side_graphics(28);
		tigers_side_graphics();

		char choice = 'k';
		while (true) {
			gotoxy(100, 45);

			char move;
			move = _getch();
			if (move == -32) //znak specjalny
				move = _getch(); //strzalka

			goats_side_graphics();
			tigers_side_graphics();

			switch (move) {
			case 72: //góra
			case 80: //do³
				if (choice == 'k') {
					choice = 't';
					tigers_side_graphics(28);
				}
				else {
					choice = 'k';
					goats_side_graphics(28);
				}
				break;

			case '\r':
			case '\n': //bo dla windowsa enter to \r\n
				return choice;
				break;

			default:
				switch (choice) {
				case 'k': goats_side_graphics(28); break;
				case 't': tigers_side_graphics(28); break;
				}
				break;
			}
		}
	}
	else { //multiplayer
		pick_a_side_graphics();
		goats_side_left_graphics(28);
		tigers_side_left_graphics(28);
		goats_side_right_graphics();
		tigers_side_right_graphics();

		char choice = 'k';
		while (true) {
			gotoxy(100, 45);

			char move;
			move = _getch();
			if (move == -32) //znak specjalny
				move = _getch(); //strzalka

			goats_side_left_graphics();
			tigers_side_left_graphics();
			goats_side_right_graphics();
			tigers_side_right_graphics();

			switch (move) {
			case 75: //lewo
			case 77: //prawo
				if (choice == 'k') {
					choice = 't';
					goats_side_right_graphics(28);
					tigers_side_right_graphics(28);
				}
				else {
					choice = 'k';
					goats_side_left_graphics(28);
					tigers_side_left_graphics(28);
				}
				break;

			case '\r':
			case '\n': //bo dla windowsa enter to \r\n
				return choice;
				break;

			default:
				switch (choice) {
				case 'k':
					goats_side_left_graphics(28);
					tigers_side_left_graphics(28);
					break;
				case 't':
					goats_side_right_graphics(28);
					tigers_side_right_graphics(28);
					break;
				}
				break;
			}
		}
	}
}

void set_nicknames(char* nickname1, char* nickname2, char mode) {
	system("cls");
	if (mode == 's') { //singleplayer
		set_nickname_graphics(0);
		do {
			gotoxy(43, 19);
			cout << "_______________" << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			get_nickname(nickname1);

			char tmp[16] = { '\0' };
			for (int i = 0; i < 15; i++)
				tmp[i] = tolower(nickname1[i]);
			if (strcmp(tmp, "komputer") == 0) {
				gotoxy(29, 22);
				cout << "To imie jest juz zarezerwowane dla komputera.";
			}
			else
				break;
		} while (true);

		strcpy(nickname2, "komputer");
	}
	else { //multiplayer
		set_nickname_graphics(1);
		do {
			gotoxy(43, 19);
			cout << "_______________" << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			get_nickname(nickname1);

			char tmp[16] = { '\0' };
			for (int i = 0; i < 15; i++)
				tmp[i] = tolower(nickname1[i]);
			if (strcmp(tmp, "komputer") == 0) {
				gotoxy(29, 22);
				cout << "To imie jest juz zarezerwowane dla komputera.";
			}
			else
				break;
		} while (true);

		system("cls");
		set_nickname_graphics(2);
		do {
			gotoxy(43, 19);
			cout << "_______________" << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			get_nickname(nickname2);

			gotoxy(29, 22);
			cout << "                                             "; //czyszczenie linijki
			
			char tmp[16] = { '\0' };
			for (int i = 0; i < 15; i++)
				tmp[i] = tolower(nickname2[i]);
			if (strcmp(tmp, "komputer") == 0) {
				gotoxy(29, 22);
				cout << "To imie jest juz zarezerwowane dla komputera.";
			}
			else if (strcmp(nickname2, nickname1) == 0) {
				gotoxy(36, 22);
				cout << "Imiona nie moga byc takie same.";
			}
			else
				break;
		} while (true);
	}
}

void get_nickname(char* nickname) {
	for (int i = 0; ; i++) {
		COORD erlier_position = wherexy();
		nickname[i] = _getch();
		if (nickname[i] > 32)
			cout << nickname[i];
		else if (nickname[i] != '\b' && nickname[i] != '\r')
			i--;

		if ((nickname[i] == '\r' || nickname[i] == '\n') && i > 0) {
			nickname[i] = '\0';
			break;
		}
		else if (nickname[i] == '\r' || nickname[i] == '\n') {
			gotoxy(erlier_position.X + 1, erlier_position.Y + 1);
			i--;
		}

		if (nickname[i] == '\b') {
			if (i > 0) {
				cout << "\b_\b";
				i -= 2;
			}
			else
				i--;
		}

		if (i == 15) {
			cout << "\b \b";
			i--;
		}
	}
}

bool resume_game(game_info* info) {
	system("cls");
	FILE *save;
	if ((save = fopen("save.goats_and_tigers", "r")) == NULL)
		no_save_graphics();
	else {
		char mode, side, nickname1[16] = { '\0' }, nickname2[16] = { '\0' }, whose_turn, fields_values[25];
		int eaten_goats, round;
		char tmp_eaten_goats, tmp_round;
		char nickname_buffer[256];
		bool if_file_corrupted = false; //kontrola poprawnosci pliku

		fscanf(save, "%c\n", &mode);
		fscanf(save, "%c\n", &side); 
		
		fscanf(save, "%s\n", nickname_buffer);
		if (strlen(nickname_buffer) <= 15)
			strcpy(nickname1, nickname_buffer);
		else
			if_file_corrupted = true;

		fscanf(save, "%s\n", nickname_buffer);
		if (strlen(nickname_buffer) <= 15)
			strcpy(nickname2, nickname_buffer);
		else
			if_file_corrupted = true;

		fscanf(save, "%c", &whose_turn);
		for (int i = 0; i < 25; i++) {
			fscanf(save, "\n%c", &fields_values[i]);
		}
		fscanf(save, "\n%c\n", &tmp_eaten_goats);
		fscanf(save, "%c", &tmp_round);
		fclose(save);

		if (!isdigit(tmp_eaten_goats))
			if_file_corrupted = true;
		else
			eaten_goats = tmp_eaten_goats - '0';
		
		if (!isdigit(tmp_round))
			if_file_corrupted = true;
		else
			round = tmp_round - '0';
		
		if (mode != 's' && mode != 'm')
			if_file_corrupted = true;
		
		if (side != 'k' && side != 't')
			if_file_corrupted = true;
		
		if (whose_turn != 'k' &&  whose_turn != 't')
			if_file_corrupted = true;
		
		int goats = 0, tigers = 0;
		for (int i = 0; i < 25; i++) {
			if (fields_values[i] != 'K' && fields_values[i] != 'T' && fields_values[i] != char(219))
				if_file_corrupted = true;
			if (fields_values[i] == 'K')
				goats++;
			if (fields_values[i] == 'T')
				tigers++;
		}
		
		if (tigers != 4)
			if_file_corrupted = true;
		
		if (eaten_goats > 5 || eaten_goats < 0)
			if_file_corrupted = true;
		
		if ((goats + eaten_goats) > 20)
			if_file_corrupted = true;
		
		if (round != 1 && round != 2)
			if_file_corrupted = true;

		if (if_file_corrupted == true) {
			corrupted_file_graphics();

			system("attrib -H save.goats_and_tigers"); //usuniecie atrybutu ukrycia
			system("attrib -R save.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
			remove("save.goats_and_tigers");

			gotoxy(58, 44);
			change_color(28);
			cout << "nacisnij backspace aby wrocic do menu...";
			change_color(26);
			char back;
			while ((back = _getch()) != '\b');
			system("cls");
			return false;
		}

		info->mode = mode;
		info->side = side;
		strcpy(info->nickname1, nickname1);
		strcpy(info->nickname2, nickname2);
		info->whose_turn = whose_turn;
		for (int i = 0; i < 25; i++) {
			info->fields_values[i] = fields_values[i];
		}
		info->eaten_goats = eaten_goats;
		info->round = round;

		system("attrib -H save.goats_and_tigers"); //usuniecie atrybutu ukrycia
		system("attrib -R save.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
		remove("save.goats_and_tigers");
		return true;
	}

	gotoxy(58, 44);
	change_color(28);
	cout << "nacisnij backspace aby wrocic do menu...";
	change_color(26);
	char back;
	while ((back = _getch()) != '\b');
	system("cls");
	return false;
}

void stats(void) {
	system("cls");
	cout << "\n\n\n";

	cout << "                                          TABLICA WYNIKOW\n\n";

	FILE *stats = fopen("stats.goats_and_tigers", "r");
	char nickname[16], nickname_buffer[256];
	int wins, losses;
	bool if_file_corrupted = false; //kontrola poprawnosci pliku

	if (fscanf(stats, "%s %d %d", nickname_buffer, &wins, &losses) != 3)
		cout << "                                            brak danych";
	else {
		cout << "                        Gracz                 Wygrane                 Przegrane\n\n";
		do
		{
			if (wins < 0 || losses < 0) {
				if_file_corrupted = true;
				break;
			}

			if (strlen(nickname_buffer) <= 15)
				strcpy(nickname, nickname_buffer);
			else {
				if_file_corrupted = true;
				break;
			}

			COORD c = wherexy();
			c.Y++;
			int spacje, tmp;

			gotoxy(27 - (strlen(nickname) - 1) / 2, c.Y); //26 znak to srodek slowa Gracz
			cout << nickname;

			tmp = wins / 10;
			for (spacje = 0; tmp != 0; spacje++)
				tmp /= 10;
			gotoxy(50 - spacje, c.Y); //49 znak to srodek slowa Wygrane
			cout << wins;

			tmp = losses / 10;
			for (spacje = 0; tmp != 0; spacje++)
				tmp /= 10;
			gotoxy(75 - spacje, c.Y); //74 znak to srodek slowa Przegrane
			cout << losses << "\n";

		} while (fscanf(stats, "%s %d %d", nickname_buffer, &wins, &losses) == 3);
	}
	fclose(stats);

	if (if_file_corrupted == true) {
		system("cls");
		corrupted_file_graphics();

		system("attrib -H stats.goats_and_tigers"); //usuniecie atrybutu ukrycia
		system("attrib -R stats.goats_and_tigers"); //usuniecie atrybutu tylko do odczytu
		remove("stats.goats_and_tigers");

		stats = fopen("stats.goats_and_tigers", "w");
		fclose(stats);
		system("attrib +R stats.goats_and_tigers"); //ustawienie atrybutu tylko do odczytu
		system("attrib +H stats.goats_and_tigers"); //ustawienie atrybutu ukrycia
	}

	gotoxy(58, 44);
	change_color(28);
	cout << "nacisnij backspace aby wrocic do menu...";
	change_color(26);
	char back;
	while ((back = _getch()) != '\b');
}

void how_to_play(void) {
	system("cls");
	cout << "\n\n\n                                             ZASADY GRY:\n\n";

	cout << "                          Jeden z graczy posiada 4 tygrysy a drugi 20 koz.\n";
	cout << "            Na poczatku gry wszystkie cztery tygrysy sa rozmieszczone w rogach planszy.\n";
	cout << "                             Kozy przy starcie sa jeszcze poza plansza.\n";
	cout << "           Tygrysy i kozy moga poruszac sie na sasiadujace wolne pola polaczone liniami.\n\n";

	cout << "                            Tygrys moze zjesc koze przeskakujac nad nia,\n";
	cout << "                        o ile za nia znajduje sie wolne pole w linii prostej.\n";
	cout << "         Tygrysy nie musza czekeac az wszystkie kozy zostana rozstawione aby moc je zjesc.\n";
	cout << "                              Tygrysy wygrywaja kiedy zjedza piec koz\n";
	cout << "                     lub kiedy zablokuja wszystkie kozy uniemozliwiajac im ruch.\n\n";

	cout << "                             Na poczatku kozy sa rozstawiane na planszy,\n";
	cout << "	                   a dopiero po rozstawieniu wszystkich mozna nimi poruszac.\n";
	cout << "                     Kozy nie moga przeskakiwac nad tygrysami ani innymi kozami.\n";
	cout << "                      Zjedzona koza opuszcza plansze i nie moze na nia wrocic.\n";
	cout << "              Kozy wygrywaja kiedy zablokuja wszystkie tygrysy uniemozliwiajac im ruch.\n";

	cout << "\n\n\n                                             STEROWANIE:\n\n";

	cout << "                             Aby poruszac sie po planszy uzywaj strzalek.\n";
	cout << "                   Aby postawic pionek przesun sie na wybrane pole i kliknij enter.\n";
	cout << "                 Aby przestawic pionek przesun sie na pole gdzie stoi wybrany pionek,\n";
	cout << "                                   kliknij enter aby go zaznaczyc,\n";
	cout << "                  po czym przesun sie na nowo wybrane pole i ponownie kliknij enter.\n";
	cout << "                      Aby anulowac wybor pionka, przesun sie na zaznaczone pole\n";
	cout << "                                      i ponownie kliknij enter.\n";
	cout << "                               Aby wyjsc w trakcie gry kliknij escape.";

	gotoxy(58, 44);
	change_color(28);
	cout << "nacisnij backspace aby wrocic do menu...";
	change_color(26);
	char back;
	while ((back = _getch()) != '\b');
}

void about(void) {
	system("cls");
	cout << "\n\n\n";

	cout << "                                           SPECYFIKACJA GRY:\n\n";

	cout << "        Ta wersja gry umozliwia granie na kwadratowej planszy 5x5, z 4 tygrysami i 20 kozami.\n";
	cout << "                 Istnieja tez inne wersje z innymi planszami i inna liczba pionkow.\n";
	cout << "          Jedna rozgrywka sklada sie z dwoch rund - w drugiej rundzie zamieniane sa strony.\n";

	cout << "\n\n\n                                              AUTOR GRY:\n\n";

	cout << "                                            Robert Saganek\n";
	cout << "\n\n\n                                          Copyright (C) 2016";

	gotoxy(58, 44);
	change_color(28);
	cout << "nacisnij backspace aby wrocic do menu...";
	change_color(26);
	char back;
	while ((back = _getch()) != '\b');
}

void end_game(void) {

}

void escape_key(bool& end_game, bool& save_game) {
	end_game = if_end();
	if (end_game == true)
		save_game = if_save();
}

bool if_end(void) {
	system("cls");
	if_end_resume_graphics(28);
	if_end_end_graphics();

	bool choice = false;
	while (true) {
		gotoxy(100, 45);

		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		if_end_resume_graphics();
		if_end_end_graphics();

		switch (move) {
		case 72: //góra
		case 80: //do³
			if (choice == false) {
				choice = true;
				if_end_end_graphics(28);
			}
			else {
				choice = false;
				if_end_resume_graphics(28);
			}
			break;

		case '\r':
		case '\n': //bo dla windowsa enter to \r\n
			system("cls");
			return choice;
			break;

		default:
			switch (choice) {
			case false: if_end_resume_graphics(28); break;
			case true: if_end_end_graphics(28); break;
			}
			break;
		}
	}
}

bool if_save(void) {
	system("cls");
	if_save_save_graphics(28);
	if_save_dont_save_graphics();

	bool choice = true;
	while (true) {
		gotoxy(100, 45);

		char move;
		move = _getch();
		if (move == -32) //znak specjalny
			move = _getch(); //strzalka

		if_save_save_graphics();
		if_save_dont_save_graphics();

		switch (move) {
		case 72: //góra
		case 80: //do³
			if (choice == true) {
				choice = false;
				if_save_dont_save_graphics(28);
			}
			else {
				choice = true;
				if_save_save_graphics(28);
			}
			break;

		case '\r':
		case '\n': //bo dla windowsa enter to \r\n
			system("cls");
			return choice;
			break;

		default:
			switch (choice) {
			case true: if_save_save_graphics(28); break;
			case false: if_save_dont_save_graphics(28); break;
			}
			break;
		}
	}
}