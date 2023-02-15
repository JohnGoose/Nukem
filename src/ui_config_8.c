/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	select_editor_ui(t_level *level)
{
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	if (level->bake_status == BAKE_BAKING)
		ui_baking(level);
	else if (level->ui.state.ui_location == UI_LOCATION_FILE_SAVE
		|| level->ui.state.ui_location == UI_LOCATION_FILE_OPEN)
		ui_render_directory(level);
	else if (level->ui.state.ui_location == UI_LOCATION_POST_PROCESS_SETTINGS)
		ui_post_process_settings(level);
	else if (editor_select(level))
		ui_editor(level);
}

void	ui_main_menu(t_window *window, t_level *level, t_game_state *game_state)
{
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	if (level->ui.main_menu == MAIN_MENU_LOCATION_MAIN)
		main_menu(level, window->ui_texture_pixels, game_state);
	else if (level->ui.main_menu == MAIN_MENU_LOCATION_SETTINGS)
	{
		ui_settings(level);
		set_text_color(UI_INFO_TEXT_COLOR);
		ui_render_info(&level->ui, level);
	}
	else if (level->ui.main_menu == MAIN_MENU_LOCATION_POST_PROCESS)
		ui_post_process_settings(level);
	else
		ui_render_directory(level);
}

void	reset_ui_state(t_ui_state *state, t_level *level)
{
	state->ui_max_width = 0;
	state->ui_text_color = 0;
	state->ui_text_x_offset = 0;
	state->ui_text_y_pos = 0;
	state->current_font = level->ui.editor_font;
}

void	ui(t_window *window, t_level *level, t_game_state *game_state)
{
	reset_ui_state(&level->ui.state, level);
	if (level->ui.state.ssp_visual)
	{
		render_ssp_visual_background(window->ui_texture_pixels);
		render_ssp_visual_text(level);
	}
	if (*game_state == GAME_STATE_EDITOR)
	{
		if (level->ui.state.ui_location == UI_LOCATION_UV_EDITOR)
			uv_editor(level, window->ui_texture_pixels);
		else if (level->ui.state.ui_location == UI_LOCATION_GAME_SETTINGS)
			game_logic_put_info(level, window->ui_texture_pixels);
		else
		{
			door_put_text(level);
			light_put_text(level);
		}
		select_editor_ui(level);
	}
	else if (*game_state == GAME_STATE_MAIN_MENU)
		ui_main_menu(window, level, game_state);
	else
		hud(level, window->ui_texture_pixels);
	ui_render_background(window, level);
	ui_render_nonfatal_errors(level);
}
