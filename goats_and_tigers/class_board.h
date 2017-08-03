#ifndef CLASS_BOARD_H
#define CLASS_BOARD_H

class board {
private:
	const COORD fields[25];
	const int fields_neighbors[25][8];
	const int fields_captures[25][8];
	char fields_values[25];
	int eaten_goats;
	int blocked_tigers;

public:
	void set_field_value(int number, char value);
	char* get_fields_values();
	void set_eaten_goats(int eaten_goats);
	int get_eaten_goats();
	void set_blocked_tigers(int blocked_tigers);
	int get_blocked_tigers();
	board();

friend class player;
friend class human_player;
friend class computer_player;
friend COORD select_field(bool&, bool&, board*, char, bool&);
friend bool if_available_move(COORD, board*);
friend bool check_if_neighbor(COORD, int, board*);
friend bool check_if_capture(COORD, int, board*);
friend bool if_can_capture(COORD, COORD, int, int&, board*);
friend bool if_goats_wins(board*, human_player*);
friend bool if_tigers_wins(board*);
friend COORD put_a_goat(board* board);
friend COORD move_a_goat(board* board);
friend COORD move_a_tiger(board* board);
friend void count_blocked_tigers(board *board);
};

#endif