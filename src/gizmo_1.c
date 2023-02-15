/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gizmo_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	is_near(int a, int b, int range)
{
	float	diff;

	diff = a - b;
	return (-range <= diff && diff <= range);
}

t_vec3	calc_move_screen_space(int index, int amount)
{
	int		get_dir[3];
	t_vec3	dir;

	get_dir[0] = 0;
	get_dir[1] = 0;
	get_dir[2] = 0;
	get_dir[index] = -1;
	dir.x = get_dir[0];
	dir.y = get_dir[1];
	dir.z = get_dir[2];
	vec_mult(&dir, amount / 100.);
	return (dir);
}

void	gizmo_call_move(t_level *level, t_vec3 move_amount)
{
	if (level->ui.state.ui_location == UI_LOCATION_DOOR_ACTIVATION_BUTTON)
		door_activation_move(level, move_amount);
	else if (level->ui.state.ui_location == UI_LOCATION_LIGHT_EDITOR)
		move_light(level, move_amount);
	else if (level->ui.state.ui_location == UI_LOCATION_GAME_SETTINGS)
		game_logic_move_selected(level, move_amount);
	else
		obj_editor_input(level, move_amount);
}

void	gizmo_set_mouse_location(t_level *level,
				int deltax, int deltay, int drag_direction)
{
	t_vec3	res;

	if (!level->ui.state.m1_drag
		&& level->ui.state.mouse_location >= MOUSE_LOCATION_GIZMO_X)
		level->ui.state.mouse_location = MOUSE_LOCATION_SELECTION;
	res = (t_vec3){0, 0, 0};
	if (deltax || deltay)
	{
		if (level->ui.state.mouse_location == MOUSE_LOCATION_GIZMO_Y)
			res = calc_move_screen_space(level->ui.state.mouse_location - 3,
					deltay * drag_direction);
		else if (level->ui.state.mouse_location == MOUSE_LOCATION_GIZMO_X
			|| level->ui.state.mouse_location == MOUSE_LOCATION_GIZMO_Z)
			res = calc_move_screen_space(level->ui.state.mouse_location - 3,
					deltax * drag_direction);
	}
	gizmo_call_move(level, res);
}

void	gizmo_move_amount(t_level *level, t_ivec2 mouse, int drag_direction)
{
	static t_ivec2	prev_mouse;

	gizmo_set_mouse_location(level,
		prev_mouse.x - mouse.x, prev_mouse.y - mouse.y, drag_direction);
	prev_mouse = mouse;
}
