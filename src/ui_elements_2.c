/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_elements_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	render_slider(unsigned int *texture, float pos, int dy, int color)
{
	int	x;
	int	y;
	int	color2;

	color2 = set_lightness(color, -0.5);
	y = -1;
	while (++y < 4)
	{
		x = -1;
		while (++x < UI_SLIDER_WIDTH)
			button_pixel_put(x + 2, y + 6 + dy, color2, texture);
	}
	render_slider_button(texture, pos, dy, color);
}

void	text(char *str)
{
	t_ui_state	*state;
	t_ivec2		moved;

	if (!str)
		return ;
	state = get_ui_state(NULL);
	moved = render_text(str, state->ui_text_x_offset, state->ui_text_y_pos);
	if (state->ui_max_width < state->ui_text_x_offset + moved.x)
		state->ui_max_width = state->ui_text_x_offset + moved.x;
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
}

int	button(int *var, char *str)
{
	t_window	*window;
	t_ui_state	*state;
	int			changed;

	window = get_window(NULL);
	state = get_ui_state(NULL);
	state->ui_text_x_offset = 14;
	render_button(window->ui_texture_pixels, var,
		state->ui_text_y_pos, state->ui_text_color);
	changed = edit_button_var(var, state);
	text(str);
	state->ui_text_x_offset = 4;
	return (changed);
}

float	clamp(float var, float min, float max)
{
	if (var < min)
		var = min;
	else if (var > max)
		var = max;
	return (var);
}

int	int_slider(int *var, char *str, int min, int max)
{
	t_window	*window;
	t_ui_state	*state;
	float		unit;
	int			res;

	window = get_window(NULL);
	state = get_ui_state(NULL);
	state->ui_text_x_offset = 4;
	if (str)
		text(str);
	state->ui_text_x_offset = 14;
	*var = clamp(*var, min, max);
	*var -= min;
	unit = (float)*var / (float)(max - min);
	render_slider(window->ui_texture_pixels, unit,
		state->ui_text_y_pos, state->ui_text_color);
	res = edit_slider_var(&unit, state);
	unit = clamp(unit, 0, 1);
	*var = min + ((max - min) * unit);
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
	return (res);
}
