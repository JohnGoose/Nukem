/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/20 15:55:50 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	render_text_3d(char *str, t_vec3 pos,
			unsigned int color, t_level *level)
{
	set_text_color(color);
	camera_offset(&pos, &level->cam);
	if (pos.z > 0)
		render_text(str, pos.x, pos.y);
}

void	render_ssp_visual_background(unsigned int *texture)
{
	int	x;
	int	y;

	y = -1;
	while (++y < RES_Y)
	{
		x = -1;
		while (++x < RES_X)
		{
			if (!texture[x + (y * RES_X)])
			{
				if (get_ssp_coordinate(x, 1) % 2
					^ get_ssp_coordinate(y, 0) % 2)
					button_pixel_put(x, y, SSP_VISUAL_CHESSBOARD_1, texture);
				else
					button_pixel_put(x, y, SSP_VISUAL_CHESSBOARD_2, texture);
			}
		}
	}
}

void	ssp_visual_text_text(t_level *level,
				int max_tris, int x, int y)
{
	char	buf[100];
	int		color;

	ft_sprintf(buf, "%d", level->ssp[y * SSP_MAX_X + x].tri_amount);
	color = (float)level->ssp[y * SSP_MAX_X + x].tri_amount
		/ max_tris * 0xff;
	color = crossfade(0x00ff00, 0xff0000, color, 0xff);
	set_text_color(color);
	render_text(buf,
		(RES_X / SSP_MAX_X) * x + (RES_X / SSP_MAX_X / 2) - 5,
		(RES_Y / SSP_MAX_Y) * y + (RES_Y / SSP_MAX_Y / 2) - 7);
}

void	render_ssp_visual_text(t_level *level)
{
	int	max_tris;
	int	x;
	int	y;

	max_tris = 0;
	y = -1;
	while (++y < SSP_MAX_Y)
	{
		x = -1;
		while (++x < SSP_MAX_X)
			if (max_tris < level->ssp[y * SSP_MAX_X + x].tri_amount)
				max_tris = level->ssp[y * SSP_MAX_X + x].tri_amount;
	}
	y = -1;
	while (++y < SSP_MAX_Y)
	{
		x = -1;
		while (++x < SSP_MAX_X)
			ssp_visual_text_text(level, max_tris, x, y);
	}
}

void	ui_render_background(t_window *window, t_level *level)
{
	int	x;
	int	y;

	y = -1;
	while (++y < level->ui.state.ui_text_y_pos + 6 && y < RES_Y)
	{
		x = -1;
		while (++x < level->ui.state.ui_max_width + UI_PADDING_4
			&& x < RES_X)
		{
			if (!window->ui_texture_pixels[x + (y * RES_X)])
				button_pixel_put(x, y, UI_BACKGROUND_COL,
					window->ui_texture_pixels);
		}
	}
}
