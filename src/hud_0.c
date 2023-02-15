/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

unsigned int	cycle_rgb(unsigned int time)
{
	unsigned int	red;
	unsigned int	grn;
	unsigned int	blu;

	red = 0xff * ((sin((time / (CYCLE_RGB_LOOP_FPS * 1000.0))
					+ (M_PI / 1.5 * 0)) + 1) / 2);
	grn = 0xff * ((sin((time / (CYCLE_RGB_LOOP_FPS * 1000.0))
					+ (M_PI / 1.5 * 1)) + 1) / 2);
	blu = 0xff * ((sin((time / (CYCLE_RGB_LOOP_FPS * 1000.0))
					+ (M_PI / 1.5 * 2)) + 1) / 2);
	red = red << 8 * 3;
	grn = grn << 8 * 2;
	blu = blu << 8 * 1;
	return (red + grn + blu + 0x70);
}

void	pixel_put_hud(int x, int y, int color, unsigned int *texture)
{
	if (x < 0 || y < 0 || x >= RES_X || y >= RES_Y)
		return ;
	texture[x + (y * RES_X)] = color;
}

void	death_overlay(unsigned int *pixels)
{
	int	x;
	int	y;

	x = 0;
	while (x < RES_X)
	{
		y = 0;
		while (y < RES_Y)
		{
			pixel_put_hud(x, y, DEATH_OVERLAY_COLOR, pixels);
			y++;
		}
		x++;
	}
}

void	crosshair(unsigned int *pixels, int x, int y)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 10)
	{
		j = -1;
		while (++j < 3)
		{
			pixel_put_hud(x + i - 14, y + j, CROSSHAIR_COLOR, pixels);
			pixel_put_hud(x + i + 4, y + j, CROSSHAIR_COLOR, pixels);
		}
	}
	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 10)
		{
			pixel_put_hud(x + i - 1, y + j - 14, CROSSHAIR_COLOR, pixels);
			pixel_put_hud(x + i - 1, y + j + 6, CROSSHAIR_COLOR, pixels);
		}
	}
}

void	ingame_hud(t_level *level, unsigned int *pixels)
{
	char	buf[100];

	level->ui.state.current_font = level->ui.hud_font;
	set_text_color(HUD_TEXT_COLOR);
	crosshair(pixels, RES_X / 2, RES_Y / 2);
	ft_sprintf(buf, "%d+", level->game_logic.player.health);
	render_text(buf, HUD_FONT_SIZE / 4, RES_Y - HUD_FONT_SIZE);
	ft_sprintf(buf, "%d", level->game_logic.player.ammo);
	render_text(buf, RES_X - ((HUD_FONT_SIZE / 2) * strlen(buf)),
		RES_Y - HUD_FONT_SIZE);
}
