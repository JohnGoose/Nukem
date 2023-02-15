/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_menu_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	main_menu_text_background(t_rect rect, unsigned int *pixels)
{
	int	x;
	int	y;

	x = rect.x;
	while (x < rect.x + rect.w)
	{
		y = rect.y;
		while (y < rect.y + rect.h)
		{
			if (x >= 0 && y >= 0 && x < RES_X && y < RES_Y)
				pixels[x + (y * RES_X)] = MAIN_MENU_FONT_BACKGROUND_COLOR;
			y++;
		}
		x++;
	}
}

void	main_menu_title(t_bmp *img, unsigned int *pixels)
{
	static float	amount = 0;

	amount += .02;
	fake_analog_signal(img, pixels, sinf(amount) / 2 + 0.5);
}

int	mouse_collision(t_rect rect, t_ivec2 mouse)
{
	if (mouse.x >= rect.x && mouse.x < rect.x + rect.w
		&& mouse.y >= rect.y && mouse.y < rect.y + rect.h)
		return (TRUE);
	return (FALSE);
}

int	main_menu_button_text(char *text, int index,
					t_level *level, unsigned int *pixels)
{
	t_rect	rect;
	t_ivec2	res;

	rect.x = MAIN_MENU_FONT_SIZE * 2;
	rect.y = RES_Y / 2
		+ ((MAIN_MENU_FONT_SIZE * MAIN_MENU_FONT_PADDING_MULTIPLIER)
			* (index - (MAIN_MENU_BUTTON_AMOUNT / 2)));
	res = render_text(text, rect.x, rect.y);
	rect.w = res.x;
	rect.h = res.y;
	rect.x -= MAIN_MENU_FONT_SIZE / 4;
	rect.w += MAIN_MENU_FONT_SIZE / 2;
	if (mouse_collision(rect, level->ui.state.mouse))
	{
		main_menu_text_background(rect, pixels);
		if (level->ui.state.m1_click)
			return (TRUE);
	}
	return (FALSE);
}

void	fix_faces(t_level *level)
{
	int	i;

	i = 0;
	while (i < level->all.tri_amount)
	{
		if (level->all.tris[i].isbroken)
			level->all.tris[i].isbroken = FALSE;
		i++;
	}
}
