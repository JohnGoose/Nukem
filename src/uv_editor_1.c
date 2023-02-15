/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_editor_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	update_uv_closest_vertex(t_level *level,
				float image_scale, t_ivec2 offset, t_ivec2 mouse)
{
	t_vec2	*closest;

	closest = find_closest_to_mouse(level, NULL, NULL, NULL);
	if (closest && level->ui.state.mouse_location == MOUSE_LOCATION_UV_EDITOR
		&& level->ui.state.m1_drag)
	{
		mouse.x -= offset.x;
		mouse.y -= offset.y;
		closest->x = mouse.x / (level->texture.width * image_scale);
		closest->y = 1 - (mouse.y / (level->texture.height * image_scale));
	}
}

void	set_fourth_vertex_uv(t_tri *a)
{
	t_vec2	shared1;
	t_vec2	shared2;
	t_vec2	avg;
	t_vec2	new;
	t_vec2	res;

	vec2_sub(&shared1, a->verts[1].txtr, a->verts[0].txtr);
	vec2_sub(&shared2, a->verts[2].txtr, a->verts[0].txtr);
	vec2_avg(&avg, shared1, shared2);
	vec2_add(&new, avg, avg);
	vec2_add(&res, new, a->verts[0].txtr);
	a->verts[3].txtr.x = res.x;
	a->verts[3].txtr.y = res.y;
}

int	get_uv_wf_line_settings(int k, t_vec2 *start, t_vec2 *stop,
			t_uv_parameters param)
{
	static int	arr[4] = {1, 3, 0, 2};
	int			next;

	if (param.tri->isquad)
		next = arr[k];
	else
		next = (k + 1) % 3;
	start->x = param.scale.x * param.tri->verts[k].txtr.x;
	start->y = param.scale.y * (1 - param.tri->verts[k].txtr.y);
	stop->x = param.scale.x * param.tri->verts[next].txtr.x;
	stop->y = param.scale.y * (1 - param.tri->verts[next].txtr.y);
	vec2_add(start, *start, param.offset);
	vec2_add(stop, *stop, param.offset);
	return (next);
}

t_ivec2	get_uv_wf_line_color(int k, int next)
{
	t_ivec2	color;

	color.x = WF_SELECTED_COL >> 8;
	color.y = WF_SELECTED_COL >> 8;
	if (k == 3)
		color.x = WF_UNSELECTED_COL >> 8;
	if (next == 3)
		color.y = WF_UNSELECTED_COL >> 8;
	return (color);
}

void	draw_face_uv(t_level *level, t_uv_parameters param,
													t_ivec2 mouse)
{
	t_ivec2	color;
	t_vec2	start;
	t_vec2	stop;
	int		k;
	int		next;

	k = 0;
	start.x = 0;
	start.y = 0;
	while (k < 3 + param.tri->isquad)
	{
		next = get_uv_wf_line_settings(k, &start, &stop, param);
		color = get_uv_wf_line_color(k, next);
		uv_print_line(start, stop, color, param.pixels);
		if (k == 3)
			put_uv_vertex(start, 0xff, param.pixels);
		else
		{
			set_fourth_vertex_uv(param.tri);
			put_uv_vertex(start, WF_SELECTED_COL, param.pixels);
			find_closest_to_mouse(
				level, &param.tri->verts[k].txtr, &start, &mouse);
		}
		k++;
	}
}
