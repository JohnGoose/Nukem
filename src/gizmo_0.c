/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gizmo_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	gizmo_pixel_put(int x, int y, int color, unsigned int *texture)
{
	if (x >= 0 && y >= 0 && x < RES_X && y < RES_Y)
		texture[x + (y * RES_X)] = color;
}

void	gizmo_print_line(t_vec3 start, t_vec3 stop, int color,
												unsigned int *pixels)
{
	t_vec3	step;
	t_vec3	pos;
	int		i;

	i = 0;
	if (start.z < 0 || stop.z < 0)
		return ;
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
		gizmo_pixel_put(pos.x, pos.y, color, pixels);
		pos.x += step.x;
		pos.y += step.y;
		pos.z++;
		i++;
	}
}

t_vec3	vec_sub_return(t_vec3 ve1, t_vec3 ve2)
{
	t_vec3	res;

	res.x = ve1.x - ve2.x;
	res.y = ve1.y - ve2.y;
	res.z = ve1.z - ve2.z;
	return (res);
}

void	put_gizmo_box(t_vec3 vertex, int color, unsigned int *pixels)
{
	int	a;
	int	b;

	if (vertex.z < 0)
		return ;
	a = -6;
	while (a < 11)
	{
		b = -6;
		while (b < 11)
		{
			gizmo_pixel_put(vertex.x + a, vertex.y + b, color, pixels);
			b++;
		}
		a++;
	}
}

void	gizmo_render(t_level *level, unsigned int *pixels)
{
	t_vec3	avg;
	t_vec3	x;
	t_vec3	y;
	t_vec3	z;

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
	gizmo_print_line(avg, x, 0xff0000ff, pixels);
	gizmo_print_line(avg, y, 0x00ff00ff, pixels);
	gizmo_print_line(avg, z, 0x0000ffff, pixels);
	put_gizmo_box(x, 0xff0000ff, pixels);
	put_gizmo_box(y, 0x00ff00ff, pixels);
	put_gizmo_box(z, 0x0000ffff, pixels);
}
