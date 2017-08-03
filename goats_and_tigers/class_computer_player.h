#ifndef CLASS_COMPUTER_PLAYER_H
#define CLASS_COMPUTER_PLAYER_H

class computer_player : public player {
public:
	COORD move(COORD before_position, board* board);

	void blockade() {};
};

#endif