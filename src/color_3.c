/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	sharpen(unsigned int *pixels, unsigned int *buf, float amount)
{
	float	neighbor;
	float	center;
	int		x;
	int		y;

	neighbor = amount * -1.0;
	center = amount * 4.0 + 1.0;
	ft_memcpy(buf, pixels, sizeof(int) * RES_X * RES_Y);
	y = 1;
	while (y < RES_Y - 1)
	{
		x = 1;
		while (x < RES_X - 1)
		{
			pixels[x + y * RES_X]
				= sharpen_kernel(buf, x + y * RES_X, center, neighbor);
			x++;
		}
		y++;
	}
}

void	post_process(t_window *window, t_level *level)
{
	if (level->ui.blur)
		blur_pixels(window->frame_buffer, level->ui.raycast_quality);
	if (level->ui.bloom_intensity)
		bloom(level, window);
	if (level->ui.ssao_radius && level->ui.ssao_intensity)
		ssao(window, level);
	fill_pixels(window->frame_buffer, level->ui.raycast_quality,
		level->ui.smooth_pixels);
	if (level->ui.chromatic_abberation)
		chromatic_abberation(window->frame_buffer, window->post_process_buf,
			level->ui.chromatic_abberation);
	if (level->ui.sharpen)
		sharpen(window->frame_buffer, window->post_process_buf,
			level->ui.sharpen);
}

SDL_Color	get_sdl_color(unsigned int color)
{
	SDL_Color	res;

	res.r = (color << 8 * 0) >> 8 * 3;
	res.g = (color << 8 * 1) >> 8 * 3;
	res.b = (color << 8 * 2) >> 8 * 3;
	res.a = (color << 8 * 3) >> 8 * 3;
	return (res);
}

t_vec3	get_normal(int vec)
{
	unsigned char	*v;
	t_vec3			dir;

	v = (unsigned char *)&vec;
	dir.x = v[3] - 128;
	dir.y = -(v[1] - 128);
	dir.z = v[2] - 128;
	return (dir);
}

void	wrap_coords_inverted(int *x, int *y, int max_x, int max_y)
{
	if (*y < 0)
		*y = max_y - (*y % max_y);
	if (*y >= max_y)
		*y = *y % max_y;
	*y = max_y - *y - 1;
	if (*x < 0)
		*x = max_x - (*x % max_x);
	if (*x >= max_x)
		*x = *x % max_x;
}
