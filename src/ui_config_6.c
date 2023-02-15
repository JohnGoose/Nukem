/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_light_editor(t_level *level)
{
	bake_buttons(level);
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	if (call("close light editor", NULL))
	{
		level->ui.state.ui_location = UI_LOCATION_MAIN;
		level->selected_light_index = 0;
	}
	button(&level->ui.raytracing, "raytrace lights");
	ui_level_light_settings(level);
	call("add light", &add_light);
	ui_single_light_settings(level);
	set_text_color(UI_INFO_TEXT_COLOR);
	ui_render_info(&level->ui, level);
}

void	ui_game_settings_delete_selected(t_level *level)
{
	if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_MENU_ANIMATION)
	{
		if (call("delete node", NULL))
			camera_path_delete_pos(&level->main_menu_anim,
				level->ui.state.logic_selected_index);
	}
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_AMMO)
		call("delete ammo box", &delete_ammo_box);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_HEALTH)
		call("delete health box", &delete_health_box);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_ENEMY)
		call("delete enemy", &delete_enemy);
}

void	ui_animation_settings(t_level *level)
{
	char	buf[100];

	float_slider(&level->game_logic.item_spin_speed, "item spin speed", 0, 0.1);
	int_slider(&level->game_logic.enemy_animation_view_index,
		"view enemy animation", -1, 3);
	level->game_logic.enemy_animation_view_index
		= clamp(level->game_logic.enemy_animation_view_index, 0, 2);
	if (level->game_logic.enemy_animation_view_index == 0)
	{
		ft_sprintf(buf, "enemy animation speed %.2fx",
			level->game_models.enemy_run.duration_multiplier);
		float_slider(&level->game_models.enemy_run.duration_multiplier,
			buf, 0, 2.0);
	}
	else if (level->game_logic.enemy_animation_view_index == 1)
	{
		ft_sprintf(buf, "enemy animation speed %.2fx",
			level->game_models.enemy_die.duration_multiplier);
		float_slider(&level->game_models.enemy_die.duration_multiplier,
			buf, 0, 2.0);
	}
}

void	ui_game_settings(t_level *level)
{
	char	buf[100];

	if (call("close", NULL))
		level->ui.state.ui_location = UI_LOCATION_MAIN;
	ft_sprintf(buf, "win distance: %.2fm", level->game_logic.win_dist);
	float_slider(&level->game_logic.win_dist, buf, 1, 40);
	call("set win position", &set_win_pos);
	call("set spawn position", &set_spawn_pos);
	if (call("menu add camera pos", NULL))
		camera_path_add_pos(&level->main_menu_anim, level->cam);
	ft_sprintf(buf, "main menu animation time %ds",
		level->main_menu_anim.duration);
	button(&level->main_menu_anim.loop, "main menu anim edge loop");
	int_slider((int *)&level->main_menu_anim.duration, buf, 2, 50);
	if (call("enemy and damage settings", NULL))
		level->ui.state.ui_location
			= UI_LOCATION_ENEMY_AND_DAMAGE_SETTINGS;
	call("add enemy", &add_enemy);
	call("add ammo box", &add_ammo_box);
	call("add health box", &add_health_box);
	ui_game_settings_delete_selected(level);
	ui_animation_settings(level);
	set_text_color(UI_INFO_TEXT_COLOR);
	ui_render_info(&level->ui, level);
}

void	choose_shader(t_tri *tri)
{
	text("shader:");
	if (call("none", NULL))
	{
		free(tri->perlin);
		tri->perlin = NULL;
		tri->shader = SHADER_NONE;
	}
	if (call("perlin", NULL))
	{
		if (!tri->perlin)
		{
			perlin_init(tri);
			noise2(0, 0);
		}
		tri->shader = SHADER_PERLIN;
	}
	if (call("rule 30", NULL))
		tri->shader = SHADER_RULE_30;
}
