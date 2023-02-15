/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	skybox(t_level *l, t_cast_result *res)
{
	t_color	b;
	t_color	sun;
	t_obj	*obj;
	float	fade;
	int		i;

	obj = &l->sky.visible;
	if (res->reflection_depth)
		obj = &l->sky.all;
	b = get_skybox_brightness(l);
	skybox_reset_result(l, res);
	fade = skybox_sun_fade(&sun, res, l);
	i = 0;
	while (i < obj->tri_amount && 0 >= cast_face(obj->tris[i], res->ray, res))
		i++;
	if (i == obj->tri_amount)
		return ;
	face_color(res->uv.x, res->uv.y, obj->tris[i], res);
	if (fade)
		res->color = crossfade(brightness(res->color >> 8, b) >> 8,
				color_to_int(sun) >> 8, fade * 0xff, 0xff);
	else
		res->color = brightness(res->color >> 8, b) + 0xff;
}

void	fog(unsigned int *color, float dist, unsigned int fog_color,
													t_level *level)
{
	float	fade;

	if (dist < level->ui.render_distance)
	{
		fade = (dist + 1) / (level->ui.render_distance - 1);
		if (fade > 1)
			fade = 1;
		*color = crossfade(*color >> 8, fog_color >> 8, 0xff * fade, 0xff);
	}
	else
		*color = fog_color;
}

void	blur_pixels(unsigned int *color, int gap)
{
	int		res;
	int		x;
	int		y;
	int		col;

	y = gap;
	while (y < RES_Y - gap)
	{
		x = gap;
		while (x < RES_X - gap)
		{
			res = color[x + (y * RES_X)];
			col = color[x - gap + (y * RES_X)];
			res = crossfade(res >> 8, col >> 8, .25 * 0xff, 0xff);
			col = color[x + ((y - gap) * RES_X)];
			res = crossfade(res >> 8, col >> 8, .25 * 0xff, 0xff);
			col = color[x + gap + (y * RES_X)];
			res = crossfade(res >> 8, col >> 8, .25 * 0xff, 0xff);
			col = color[x + ((y + gap) * RES_X)];
			res = crossfade(res >> 8, col >> 8, .25 * 0xff, 0xff);
			color[x + (y * RES_X)] = res;
			x += gap;
		}
		y += gap;
	}
}

int	smooth_color_kernel(unsigned int *pixels, int gap, int x, int y)
{
	int	dx;
	int	dy;
	int	re1;
	int	re2;
	int	tmp;

	dx = x - x % gap;
	dy = y - y % gap;
	re1 = pixels[dx + dy * RES_X];
	re2 = pixels[dx + (dy + gap) * RES_X];
	tmp = crossfade(re1 >> 8, re2 >> 8, y % gap / (float)gap * 0xff, 0xff);
	re1 = pixels[dx + gap + dy * RES_X];
	re2 = pixels[dx + gap + (dy + gap) * RES_X];
	re1 = crossfade(re1 >> 8, re2 >> 8, y % gap / (float)gap * 0xff, 0xff);
	return (crossfade(tmp >> 8, re1 >> 8, x % gap / (float)gap * 0xff, 0xff));
}

int	smooth_color(unsigned int *pixels, int gap, int x, int y)
{
	int	dx;
	int	dy;
	int	re1;
	int	re2;

	dx = x - x % gap;
	dy = y - y % gap;
	if (x >= RES_X - gap && y >= RES_Y - gap)
		return (pixels[dx + ((y - y % gap) * RES_X)]);
	if (x >= RES_X - gap)
	{
		re1 = pixels[dx + dy * RES_X];
		re2 = pixels[dx + (dy + gap) * RES_X];
		return (crossfade(re1 >> 8, re2 >> 8, y % gap
				/ (float)gap * 0xff, 0xff));
	}
	if (y >= RES_Y - gap)
	{
		re1 = pixels[dx + dy * RES_X];
		re2 = pixels[dx + gap + dy * RES_X];
		return (crossfade(re1 >> 8, re2 >> 8, x % gap
				/ (float)gap * 0xff, 0xff));
	}
	return (smooth_color_kernel(pixels, gap, x, y));
}
