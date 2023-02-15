/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_menu_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 21:30:28 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	start_game(t_game_state *game_state, t_level *level)
{
	t_window	*window;

	level->ui.noclip = FALSE;
	*game_state = GAME_STATE_INGAME;
	ui_go_back(level, game_state);
	*game_state = GAME_STATE_INGAME;
	obj_copy(&level->game_models.viewmodel,
		&level->game_models.reload_animation.keyframes[0]);
	spawn_enemies(level);
	reset_doors(level);
	Mix_PlayMusic(level->audio.game_music, -1);
	level->ui.state.mouse_capture = TRUE;
	window = get_window(NULL);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

int	main_menu_buttons_level(t_game_state *game_state,
						t_level *level, unsigned int *pixels)
{
	if (main_menu_button_text("play", 0, level, pixels)
		&& level->bake_status != BAKE_BAKING)
		start_game(game_state, level);
	else if (main_menu_button_text("edit level", 1, level, pixels))
	{
		level->ui.noclip = TRUE;
		Mix_HaltMusic();
		*game_state = GAME_STATE_EDITOR;
		level->ui.state.ui_location = UI_LOCATION_MAIN;
	}
	else
		return (0);
	level->ui.state.m1_click = FALSE;
	fix_faces(level);
	return (1);
}

void	main_menu_buttons_other(t_game_state *game_state, int *state_changed,
						t_level *level, unsigned int *pixels)
{
	if (main_menu_button_text("select level", 2, level, pixels))
	{
		level->ui.state.ui_location = UI_LOCATION_FILE_OPEN;
		level->ui.main_menu = MAIN_MENU_LOCATION_LEVEL_SELECT;
		ft_strcpy(level->ui.state.extension, ".doom-nukem");
		level->ui.state.open_file = &open_level;
	}
	if (main_menu_button_text("create level", 3, level, pixels))
	{
		Mix_HaltMusic();
		create_default_level(level);
		level->ui.noclip = TRUE;
		*game_state = GAME_STATE_EDITOR;
		*state_changed = TRUE;
		level->level_initialized = TRUE;
	}
	if (main_menu_button_text("settings", 4, level, pixels))
		level->ui.main_menu = MAIN_MENU_LOCATION_SETTINGS;
}

void	main_menu(t_level *level, unsigned int *pixels,
			t_game_state *game_state)
{
	int	state_changed;

	level->ui.state.current_font = level->ui.main_menu_font;
	set_text_color(MAIN_MENU_FONT_COLOR);
	main_menu_title(&level->main_menu_title, pixels);
	state_changed = FALSE;
	level->ui.state.ui_max_width = 0;
	if (level->level_initialized)
		state_changed += main_menu_buttons_level(game_state, level, pixels);
	main_menu_buttons_other(game_state, &state_changed, level, pixels);
	if (state_changed)
	{
		level->cam.pos = level->game_logic.spawn_pos.pos;
		level->cam.look_side = level->game_logic.spawn_pos.look_side;
		level->cam.look_up = level->game_logic.spawn_pos.look_up;
		level->game_logic.player.vel.x = 0;
		level->game_logic.player.vel.y = 0;
		level->game_logic.player.vel.z = 0;
	}
}