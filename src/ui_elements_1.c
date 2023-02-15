/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_elements_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	edit_button_var(int *var, t_ui_state *state)
{
	int	x;
	int	y;

	SDL_GetMouseState(&x, &y);
	if (state->mouse_location == MOUSE_LOCATION_UI && state->m1_click
		&& x >= 2 && x <= 11 && y >= state->ui_text_y_pos
		&& y <= state->ui_text_y_pos + UI_ELEMENT_HEIGHT)
	{
		*var = *var == 0;
		return (TRUE);
	}
	return (FALSE);
}

t_rect	render_call(t_ivec2 *size, t_ui_state *state)
{
	t_window	*window;
	t_rect		rect;
	int			color;
	int			x;
	int			y;

	window = get_window(NULL);
	rect.x = state->ui_text_x_offset - 2;
	rect.y = state->ui_text_y_pos + 1;
	rect.w = size->x + 4;
	rect.h = size->y - 1;
	color = state->ui_text_color;
	if (mouse_collision(rect, state->mouse))
		color = set_lightness(color, -0.1);
	y = rect.y - 1;
	while (++y < rect.y + rect.h)
	{
		x = rect.x - 1;
		while (++x < rect.x + rect.w)
			button_pixel_put(x, y, color, window->ui_texture_pixels);
	}
	return (rect);
}

void	render_button(unsigned int *texture, int *var, int dy, int color)
{
	int	edge_color;
	int	x;
	int	y;

	edge_color = set_lightness(color, -0.8);
	if (!(*var))
		color = set_lightness(color, -0.7);
	y = 0;
	while (y < 10)
	{
		x = 0;
		while (x < 10)
		{
			if (y < 1 || y > 8 || x < 1 || x > 8)
				button_pixel_put(x + 2, y + 4 + dy, edge_color, texture);
			else
				button_pixel_put(x + 2, y + 4 + dy, color, texture);
			x++;
		}
		y++;
	}
}

void	render_slider_button(unsigned int *texture, float pos,
								int dy, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < UI_SLIDER_BUTTON_HEIGHT)
	{
		x = 0;
		while (x <= UI_SLIDER_BUTTON_WIDTH)
		{
			button_pixel_put(x + 2
				+ (int)((UI_SLIDER_WIDTH - UI_SLIDER_BUTTON_WIDTH)
					* pos), y + 2 + dy, color, texture);
			x++;
		}
		y++;
	}
}

void	render_color_slider(t_window *window, float pos,
						int dy, unsigned int *colors)
{
	int	x;
	int	y;

	y = -1;
	while (++y < 4)
	{
		x = -1;
		while (++x < UI_SLIDER_WIDTH)
			button_pixel_put(x + 2, y + 6 + dy, colors[x],
				window->ui_texture_pixels);
	}
	render_slider_button(window->ui_texture_pixels, pos, dy,
		colors[(int)(pos * (UI_SLIDER_WIDTH - 1))]);
}
