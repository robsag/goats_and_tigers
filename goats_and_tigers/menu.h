#ifndef MENU_H
#define MENU_H

void check_game_files(void);
void intro(void);
int menu();
void new_game(game_info* info);
bool resume_game(game_info* info);
void how_to_play(void);
void about(void);
void stats(void);
void end_game(void);
void escape_key(bool& end_game, bool& save_game);

#endif