#ifndef GRAPHIC_EFEECTS_H
#define GRAPHIC_EFEECTS_H

void console_settings(void);
void change_color(int color);
void change_color_intensity(int color);
void intro_graphics(void);
void new_game_graphics(int color = 26);
void resume_game_graphics(int color = 26);
void stats_graphics(int color = 26);
void how_to_play_graphics(int color = 26);
void about_graphics(int color = 26);
void end_game_graphics(int color = 26);
void menu_graphics(void);
void draw_board(char* fields_values);
void pick_a_mode_graphics(void);
void singleplayer_mode_graphics(int color = 26);
void multiplayer_mode_graphics(int color = 26);
void pick_a_side_graphics(void);
void goats_side_graphics(int color = 26);
void tigers_side_graphics(int color = 26);
void goats_side_left_graphics(int color = 26);
void tigers_side_left_graphics(int color = 26);
void goats_side_right_graphics(int color = 26);
void tigers_side_right_graphics(int color = 26);
void set_nickname_graphics(int n);
void corrupted_file_graphics(void);
void no_save_graphics(void);
void if_end_resume_graphics(int color = 26);
void if_end_end_graphics(int color = 26);
void if_save_save_graphics(int color = 26);
void if_save_dont_save_graphics(int color = 26);
void select_field_graphics(COORD point, char side, int color = 26);
void winner_graphics(char* winner);

#endif
