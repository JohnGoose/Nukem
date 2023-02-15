/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	fill_one_pixel(unsigned int *grid, int gap, t_ivec2 coord, int s)
{
	t_ivec2	limit;
	t_ivec2	i;
	int		color;

	color = grid[coord.x + coord.y * RES_X];
	limit.x = coord.x + gap;
	limit.y = coord.y + gap;
	if (limit.x > RES_X)
		limit.x -= limit.x - RES_X;
	if (limit.y > RES_Y)
		limit.y -= limit.y - RES_Y;
	i.x = coord.x;
	while (i.x < limit.x)
	{
		i.y = coord.y;
		while (i.y < limit.y)
		{
			if (s)
				grid[i.x + i.y * RES_X] = smooth_color(grid, gap, i.x, i.y);
			else
				grid[i.x + i.y * RES_X] = color;
			i.y++;
		}
		i.x++;
	}
}

void	fill_pixels(unsigned int *grid, int gap, int smooth)
{
	t_ivec2	i;

	i.y = 0;
	while (i.y < RES_Y)
	{
		i.x = 0;
		while (i.x < RES_X)
		{
			fill_one_pixel(grid, gap, i, smooth);
			i.x += gap;
		}
		i.y += gap;
	}
}

unsigned int	vec_to_color(t_vec3 color)
{
	unsigned int	res;
	unsigned char	*rgb;

	rgb = (unsigned char *)&res;
	color.x = clamp(color.x, 0, 1);
	color.y = clamp(color.y, 0, 1);
	color.z = clamp(color.z, 0, 1);
	rgb[3] = color.x * 0xff;
	rgb[2] = color.y * 0xff;
	rgb[1] = color.z * 0xff;
	rgb[0] = 0xff;
	return (res);
}

t_vec3	color_to_vec(unsigned int color)
{
	unsigned char	*rgb;
	t_vec3			res;

	rgb = (unsigned char *)&color;
	res.x = rgb[3] / (float)0xff;
	res.y = rgb[2] / (float)0xff;
	res.z = rgb[1] / (float)0xff;
	return (res);
}

unsigned int	sharpen_kernel(unsigned int *buf, int i,
						float center, float neighbor)
{
	t_vec3	tmp;
	t_vec3	c;

	c = (t_vec3){0, 0, 0};
	tmp = color_to_vec(buf[i + RES_X]);
	vec_mult(&tmp, neighbor);
	vec_add(&c, c, tmp);
	tmp = color_to_vec(buf[i - 1]);
	vec_mult(&tmp, neighbor);
	vec_add(&c, c, tmp);
	tmp = color_to_vec(buf[i]);
	vec_mult(&tmp, center);
	vec_add(&c, c, tmp);
	tmp = color_to_vec(buf[i + 1]);
	vec_mult(&tmp, neighbor);
	vec_add(&c, c, tmp);
	tmp = color_to_vec(buf[i - RES_X]);
	vec_mult(&tmp, neighbor);
	vec_add(&c, c, tmp);
	return (vec_to_color(c));
}
