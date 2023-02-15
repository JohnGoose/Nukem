/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic_editor_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	game_logic_select_nearest_to_mouse_not_item(t_level *level, float *dist)
{
	int	i;

	i = -1;
	while (++i < level->main_menu_anim.amount)
	{
		if (check_if_3d_closer(level->main_menu_anim.pos[i].pos, dist, level))
		{
			level->ui.state.logic_selected = GAME_LOGIC_SELECTED_MENU_ANIMATION;
			level->ui.state.logic_selected_index = i;
		}
	}
	i = -1;
	while (++i < level->game_logic.enemy_amount)
	{
		if (check_if_3d_closer(level->game_logic.enemies[i].spawn_pos, dist,
				level))
		{
			level->ui.state.logic_selected = GAME_LOGIC_SELECTED_ENEMY;
			level->ui.state.logic_selected_index = i;
		}
	}
}

void	game_logic_select_nearest_to_mouse(t_level *level)
{
	float	dist;

	dist = FLT_MAX;
	level->ui.state.logic_selected = GAME_LOGIC_SELECTED_NONE;
	level->ui.state.logic_selected_index = 0;
	if (check_if_3d_closer(level->game_logic.spawn_pos.pos, &dist, level))
		level->ui.state.logic_selected = GAME_LOGIC_SELECTED_SPAWN;
	if (check_if_3d_closer(level->game_logic.win_pos, &dist, level))
		level->ui.state.logic_selected = GAME_LOGIC_SELECTED_WIN;
	game_logic_select_nearest_to_mouse_item(level, &dist);
	game_logic_select_nearest_to_mouse_not_item(level, &dist);
}

void	game_logic_put_info(t_level *level, unsigned int *texture)
{
	t_vec3	dist;
	int		i;

	draw_camera_path("menu", &level->main_menu_anim, texture, level);
	render_text_3d("spawn", level->game_logic.spawn_pos.pos,
		UI_LEVEL_SETTINGS_TEXT_COLOR, level);
	vec_sub(&dist, level->game_logic.win_pos, level->cam.pos);
	if (vec_length(dist) < level->game_logic.win_dist)
		render_text_3d("win", level->game_logic.win_pos,
			UI_LEVEL_BAKED_COLOR, level);
	else
		render_text_3d("win", level->game_logic.win_pos,
			UI_LEVEL_NOT_BAKED_COLOR, level);
	i = -1;
	while (++i < level->game_logic.ammo_box_amount)
		render_text_3d("ammo", level->game_logic.ammo_box[i].pos,
			AMMO_BOX_TEXT_COLOR, level);
	i = -1;
	while (++i < level->game_logic.health_box_amount)
		render_text_3d("health", level->game_logic.health_box[i].pos,
			HEALTH_BOX_TEXT_COLOR, level);
	i = -1;
	while (++i < level->game_logic.enemy_amount)
		render_text_3d("enemy", level->game_logic.enemies[i].spawn_pos,
			ENEMY_SPAWN_TEXT_COLOR, level);
}
