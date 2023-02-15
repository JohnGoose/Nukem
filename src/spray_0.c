/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spray_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/15 16:26:22 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	spray_point(t_level *l, t_vec2 texture, t_vec2 steps, t_tri *tri)
{
	t_vec2			point;
	int				texture_coord;
	int				spray_coord;
	unsigned int	fade;

	texture_coord = (int)texture.x + (int)texture.y * l->texture.width;
	spray_coord = (int)(l->spray.width * steps.x)
		+ (int)(l->spray.height * steps.y) * l->spray.width;
	if (spray_coord >= l->spray.width * l->spray.height || spray_coord < 0)
		return ;
	point.x = texture.x / l->texture.width;
	point.y = 1 - texture.y / l->texture.height;
	fade = ((unsigned int )l->spray.image[spray_coord] << 8 * 3) >> 8 * 3;
	if (fade == 0)
		return ;
	if (point_in_tri(point,
			tri->verts[0].txtr, tri->verts[1].txtr, tri->verts[2].txtr)
		|| point_in_tri(point,
			tri->verts[3].txtr, tri->verts[1].txtr, tri->verts[2].txtr))
		l->spray_overlay[texture_coord]
			= crossfade(l->texture.image[texture_coord] >> 8,
				l->spray.image[spray_coord] >> 8, fade, 0xff);
}

void	spray_line(t_level *l, t_vec2 line[2], t_tri *tri, t_vec2 axis_step)
{
	t_vec2	increment;
	t_vec2	texture;
	int		steps;
	int		i;

	texture.x = line[0].x;
	texture.y = line[0].y;
	steps = fmax(ft_abs(line[1].x - line[0].x), ft_abs(line[1].y - line[0].y))
		* SPRAY_LINE_PRECISION;
	if (!steps)
		return ;
	increment.x = (line[1].x - line[0].x) / (float)steps;
	increment.y = (line[1].y - line[0].y) / (float)steps;
	i = -1;
	while (++i <= steps && i < 99999)
	{
		if ((int)texture.x < l->texture.width && texture.x > 0
			&& (int)texture.y < l->texture.height && texture.y > 0)
		{
			axis_step.x = (float)i / steps;
			spray_point(l, texture, axis_step, tri);
		}
		vec2_add(&texture, texture, increment);
	}
}

void	square_step(t_vec2 square[4], t_level *l, t_tri *tri, float step)
{
	t_vec2	line[2];
	t_vec2	axis_step;

	line[0] = square[1];
	line[1] = square[3];
	vec2_mult(&line[0], -step);
	vec2_mult(&line[1], -step);
	vec2_add(&line[0], square[0], line[0]);
	vec2_add(&line[1], square[2], line[1]);
	axis_step.y = step;
	spray_line(l, line, tri, axis_step);
}

void	draw_square(t_level *l, t_vec2 square[4], t_tri *tri)
{
	int		left_steps;
	int		right_steps;
	int		steps;
	int		i;

	if (ft_abs(square[1].x - square[0].x) > ft_abs(square[1].y - square[0].y))
		left_steps = ft_abs(square[1].x - square[0].x);
	else
		left_steps = ft_abs(square[1].y - square[0].y);
	if (ft_abs(square[3].x - square[2].x) > ft_abs(square[3].y - square[2].y))
		right_steps = ft_abs(square[3].x - square[2].x);
	else
		right_steps = ft_abs(square[3].y - square[2].y);
	if (!left_steps || !right_steps)
		return ;
	if (left_steps > right_steps)
		steps = left_steps;
	else
		steps = right_steps;
	steps *= SPRAY_LINE_PRECISION;
	i = -1;
	while (++i <= steps && i < 99999)
		square_step(square, l, tri, (float)i / steps);
}

t_vec2	uv_to_2d(t_tri tri, t_vec2 uv, int isquad)
{
	t_vec2	av0;
	t_vec2	av1;
	t_vec2	av2;
	t_vec2	res;

	if (isquad)
		av0 = tri.verts[3].txtr;
	else
		av0 = tri.verts[0].txtr;
	av1 = tri.verts[1].txtr;
	av2 = tri.verts[2].txtr;
	vec2_mult(&av0, 1 - uv.x - uv.y);
	vec2_mult(&av1, uv.y);
	vec2_mult(&av2, uv.x);
	res = av0;
	vec2_add(&res, res, av1);
	vec2_add(&res, res, av2);
	return (res);
}
