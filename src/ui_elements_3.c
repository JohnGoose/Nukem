/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_elements_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	float_slider(float *var, char *str, float min, float max)
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

void	generate_color_slider_saturation(unsigned int *res, int color)
{
	int	i;

	i = 0;
	while (i < UI_SLIDER_WIDTH)
	{
		res[i] = set_saturation(color, i / (float)UI_SLIDER_WIDTH);
		i++;
	}
}

void	generate_color_slider_lightness(unsigned int *res, int color)
{
	int	i;

	i = 0;
	while (i < UI_SLIDER_WIDTH)
	{
		res[i] = set_lightness(color, (i / (float)UI_SLIDER_WIDTH) * 2 - 1);
		i++;
	}
}

int	color_slider(t_color_hsl *var, char *str)
{
	t_ui_state		*state;
	unsigned int	colors[UI_SLIDER_WIDTH];
	int				res;

	state = get_ui_state(NULL);
	state->ui_text_x_offset = 4;
	text(str);
	render_color_slider(get_window(NULL), var->hue,
		state->ui_text_y_pos, state->color_slider_hue_colors);
	res = edit_slider_var(&var->hue, state);
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
	generate_color_slider_saturation(&colors[0], var->rgb_hue);
	render_color_slider(get_window(NULL), var->saturation,
		state->ui_text_y_pos, &colors[0]);
	res += edit_slider_var(&var->saturation, state);
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
	var->lightness = (var->lightness + 1) / 2;
	generate_color_slider_lightness(&colors[0], var->rgb_hue);
	render_color_slider(get_window(NULL), var->lightness,
		state->ui_text_y_pos, &colors[0]);
	res += edit_slider_var(&var->lightness, state);
	var->lightness = var->lightness * 2 - 1;
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
	hsl_update_color(var);
	return (res);
}

void	file_save(char *str, char *extension, void (*f)(t_level *, char *))
{
	t_ui_state	*state;

	state = get_ui_state(NULL);
	if (call(str, NULL))
	{
		state->ui_location = UI_LOCATION_FILE_SAVE;
		ft_strcpy(state->extension, extension);
		state->open_file = *f;
	}
}
