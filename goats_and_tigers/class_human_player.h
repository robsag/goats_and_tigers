#ifndef CLASS_HUMAN_PLAYER_H
#define CLASS_HUMAN_PLAYER_H

class human_player : public player {
public:
	void set_nickname(char* nickname);
	COORD move(COORD before_position, bool& end, bool& save, board* board, bool& escape, bool& cancel_move);

	void blockade() {};
};

bool if_can_capture(COORD selection_field, COORD target_field, int target_field_index, int& capture_field_index, board* board);

#endif