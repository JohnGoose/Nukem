/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic_editor_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 21:50:34 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	delete_ammo_box(t_level *level)
{
	int	amount;
	int	i;

	amount = level->game_logic.ammo_box_amount;
	i = level->ui.state.logic_selected_index;
	while (i < amount - 1)
	{
		level->game_logic.ammo_box[i] = level->game_logic.ammo_box[i + 1];
		i++;
	}
	level->game_logic.ammo_box
		= (t_item_pickup *)ft_realloc(level->game_logic.ammo_box,
			sizeof(t_item_pickup) * amount,
			sizeof(t_item_pickup) * (amount - 1));
	if (!level->game_logic.ammo_box)
		ft_error("memory allocation failed");
	level->game_logic.ammo_box_amount--;
	level->ui.state.logic_selected = GAME_LOGIC_SELECTED_NONE;
}

void	move_selected_vec(t_level *level, t_vec3 move_amount, t_vec3 *vec)
{
	vec_add(vec, *vec, move_amount);
	level->ui.state.gizmo_pos = *vec;
}

void	game_logic_move_selected(t_level *level, t_vec3 move)
{
	int	i;

	i = level->ui.state.logic_selected_index;
	if (i >= level->main_menu_anim.amount)
		level->ui.state.logic_selected = GAME_LOGIC_SELECTED_NONE;
	level->ui.state.gizmo_active = TRUE;
	if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_NONE)
		level->ui.state.gizmo_active = FALSE;
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_SPAWN)
		move_selected_vec(level, move, &level->game_logic.spawn_pos.pos);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_WIN)
		move_selected_vec(level, move, &level->game_logic.win_pos);
	else if (level->ui.state.logic_selected
		== GAME_LOGIC_SELECTED_MENU_ANIMATION)
		move_selected_vec(level, move, &level->main_menu_anim.pos[i].pos);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_AMMO)
		move_selected_vec(level, move, &level->game_logic.ammo_box[i].pos);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_HEALTH)
		move_selected_vec(level, move, &level->game_logic.health_box[i].pos);
	else if (level->ui.state.logic_selected == GAME_LOGIC_SELECTED_ENEMY)
		move_selected_vec(level, move, &level->game_logic.enemies[i].spawn_pos);
}

int	check_if_3d_closer(t_vec3 vert, float *dist, t_level *level)
{
	t_vec2	test;
	float	len;

	camera_offset(&vert, &level->cam);
	if (vert.z < 0 || vert.x < 0 || vert.x > RES_X || vert.y < 0
		|| vert.y > RES_Y)
		return (FALSE);
	test.x = vert.x;
	test.y = vert.y;
	test.x -= level->ui.state.mouse.x;
	test.y -= level->ui.state.mouse.y;
	len = vec2_length(test);
	if (len < *dist)
	{
		*dist = len;
		return (TRUE);
	}
	return (FALSE);
}

void	game_logic_select_nearest_to_mouse_item(t_level *level, float *dist)
{
	int	i;

	i = -1;
	i = -1;
	while (++i < level->game_logic.ammo_box_amount)
	{
		if (check_if_3d_closer(level->game_logic.ammo_box[i].pos, dist, level))
		{
			level->ui.state.logic_selected = GAME_LOGIC_SELECTED_AMMO;
			level->ui.state.logic_selected_index = i;
		}
	}
	i = -1;
	while (++i < level->game_logic.health_box_amount)
	{
		if (check_if_3d_closer(level->game_logic.health_box[i].pos, dist,
				level))
		{
			level->ui.state.logic_selected = GAME_LOGIC_SELECTED_HEALTH;
			level->ui.state.logic_selected_index = i;
		}
	}
}
