/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wireframe_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	pixel_put(int x, int y, unsigned int color, unsigned int *texture)
{
	if (x < 0 || y < 0 || x >= RES_X || y >= RES_Y)
		return ;
	if (color == WF_SELECTED_COL
		|| (texture[x + (y * RES_X)] != WF_VERT_COL
			&& texture[x + (y * RES_X)] != WF_SELECTED_COL
			&& texture[x + (y * RES_X)] != WF_VISIBLE_COL
			&& texture[x + (y * RES_X)] != WF_VISIBLE_NORMAL_COL
			&& (texture[x + (y * RES_X)] != WF_NOT_QUAD_WARNING_COL
				|| color == WF_SELECTED_COL)))
		texture[x + (y * RES_X)] = color;
}

t_vec3	move2z(t_vec3 *p1, t_vec3 *p2)
{
	t_vec3	intersection;
	float	len;

	len = p1->z / (p2->z - p1->z);
	len *= 20;
	intersection.x = p1->x + len * (p2->x - p1->x);
	intersection.y = p1->y + len * (p2->y - p1->y);
	intersection.z = 0;
	return (intersection);
}

int	z_clip_line(t_vec3 *start, t_vec3 *stop)
{
	if (start->z < 0 && stop->z < 0)
		return (TRUE);
	else if (start->z < 0)
		*start = move2z(stop, start);
	else if (stop->z < 0)
		*stop = move2z(start, stop);
	return (FALSE);
}

void	print_line(t_vec3 start, t_vec3 stop, unsigned int color,
						unsigned int *texture)
{
	t_vec3	step;
	t_vec3	pos;
	int		i;

	if (z_clip_line(&start, &stop))
		return ;
	i = 0;
	pos.x = start.x;
	pos.y = start.y;
	pos.z = 0;
	if (ft_abs(stop.x - start.x) > ft_abs(stop.y - start.y))
		step.z = ft_abs(stop.x - start.x);
	else
		step.z = ft_abs(stop.y - start.y);
	step.x = (stop.x - start.x) / (float)step.z;
	step.y = (stop.y - start.y) / (float)step.z;
	while (pos.z <= step.z && i < 10000)
	{
		pixel_put(pos.x, pos.y, color, texture);
		pos.x += step.x;
		pos.y += step.y;
		pos.z++;
		i++;
	}
}

void	rotate_vertex(float angle, t_vec3 *vertex, int axis)
{
	float	sin_angle;
	float	cos_angle;
	float	a;
	float	b;

	sin_angle = sin(angle);
	cos_angle = cos(angle);
	if (axis)
	{
		a = vertex->y;
		b = vertex->z;
		vertex->y = a * cos_angle - b * sin_angle;
		vertex->z = b * cos_angle + a * sin_angle;
	}
	else
	{
		a = vertex->x;
		b = vertex->z;
		vertex->x = a * cos_angle + b * sin_angle;
		vertex->z = b * cos_angle - a * sin_angle;
	}
}
