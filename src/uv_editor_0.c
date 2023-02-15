/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_editor_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	uv_pixel_put(int x, int y, int color, unsigned int *texture)
{
	int	opacity;

	if (x < 0 || y < 0 || x >= RES_X / 2 || y >= RES_Y)
		return ;
	opacity = (color << (8 * 3)) >> (8 * 3);
	if (!opacity)
		texture[x + (y * RES_X)] = UI_BACKGROUND_COL;
	else
		texture[x + (y * RES_X)] = color;
}

float	get_texture_scale(t_bmp *img)
{
	if (img->width < img->height)
		return ((float)
			(RES_Y - (UI_ELEMENT_HEIGHT + UI_PADDING * 2)) / img->height
		);
	else
		return ((float)RES_X / 2 / img->width);
}

void	uv_print_line(t_vec2 start, t_vec2 stop, t_ivec2 color,
												unsigned int *pixels)
{
	t_vec3	step;
	t_vec3	pos;
	int		i;

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
		uv_pixel_put(pos.x, pos.y,
			crossfade(color.x, color.y, 0xff * (pos.z / step.z), 0xff), pixels);
		pos.x += step.x;
		pos.y += step.y;
		pos.z++;
		i++;
	}
}

void	put_uv_vertex(t_vec2 vertex, int color, unsigned int *pixels)
{
	int	a;
	int	b;

	a = -1;
	while (a < 2)
	{
		b = -1;
		while (b < 2)
		{
			uv_pixel_put(vertex.x + a, vertex.y + b, color, pixels);
			b++;
		}
		a++;
	}
}

t_vec2	*find_closest_to_mouse(t_level *level, t_vec2 *txtr,
									t_vec2 *screen_pos, t_ivec2 *mouse)
{
	static float	closest_dist = -1;
	static t_vec2	*closest = NULL;
	t_vec2			*res;
	t_vec2			tmp;
	float			dist;

	if (!txtr || !screen_pos || !mouse)
	{
		res = closest;
		if (!level->ui.state.m1_drag)
		{
			closest = NULL;
			closest_dist = -1;
		}
		return (res);
	}
	tmp.x = screen_pos->x - mouse->x;
	tmp.y = screen_pos->y - mouse->y;
	dist = vec2_length(tmp);
	if (level->ui.state.m1_click && (dist < closest_dist || closest_dist == -1))
	{
		closest_dist = dist;
		closest = txtr;
	}
	return (NULL);
}
