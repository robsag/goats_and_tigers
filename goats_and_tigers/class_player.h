#ifndef CLASS_PLAYER_H
#define CLASS_PLAYER_H

class player {
private:
	char side; //kozy lub tygrysy
	bool my_turn;
	char nickname[16];

public:
	void set_side(char side);
	char get_side(void);
	void set_my_turn(bool my_turn);
	bool get_my_turn(void);
	virtual void set_nickname(char* nickname);
	char* get_nickname(void);

	virtual void blockade() = 0;

	friend class human_player;
	friend class computer_player;
};

#endif