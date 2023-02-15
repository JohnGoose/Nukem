/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gizmo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	gizmo_drag(t_level *level, t_vec3 x, t_vec3 y, t_vec3 z)
{
	static int	drag_direction = 1;
	t_ivec2		mouse;

	mouse = level->ui.state.mouse;
	if (!level->ui.state.m1_click)
		return (gizmo_move_amount(level, mouse, drag_direction));
	if (y.z > 0 && is_near(mouse.x, y.x, 9) && is_near(mouse.y, y.y, 9))
		level->ui.state.mouse_location = MOUSE_LOCATION_GIZMO_Y;
	if (y.z > 0 && is_near(mouse.x, y.x, 9) && is_near(mouse.y, y.y, 9))
		drag_direction = 1;
	else if (x.z > 0 && is_near(mouse.x, x.x, 9) && is_near(mouse.y, x.y, 9))
	{
		level->ui.state.mouse_location = MOUSE_LOCATION_GIZMO_X;
		drag_direction = 1;
		if (vec_dot((t_vec3){0, 0, -1}, level->cam.front) > 0)
			drag_direction = -1;
	}
	else if (z.z > 0 && is_near(mouse.x, z.x, 9) && is_near(mouse.y, z.y, 9))
	{
		level->ui.state.mouse_location = MOUSE_LOCATION_GIZMO_Z;
		drag_direction = -1;
		if (vec_dot((t_vec3){-1, 0, 0}, level->cam.front) > 0)
			drag_direction = 1;
	}
	gizmo_move_amount(level, mouse, drag_direction);
}

void	gizmo(t_level *level)
{
	t_vec3	x;
	t_vec3	y;
	t_vec3	z;
	t_vec3	avg;

	x = (t_vec3){-1.5, 0, 0};
	y = (t_vec3){0, -1.5, 0};
	z = (t_vec3){0, 0, -1.5};
	avg = level->ui.state.gizmo_pos;
	vec_add(&x, x, avg);
	vec_add(&y, y, avg);
	vec_add(&z, z, avg);
	camera_offset(&avg, &level->cam);
	camera_offset(&x, &level->cam);
	camera_offset(&y, &level->cam);
	camera_offset(&z, &level->cam);
	gizmo_drag(level, x, y, z);
}
