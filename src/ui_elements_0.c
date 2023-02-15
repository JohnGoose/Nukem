/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_elements_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_level	*get_level(t_level *get_level)
{
	static t_level	*level = NULL;

	if (get_level)
		level = get_level;
	else
		return (level);
	return (NULL);
}

t_window	*get_window(t_window *get_window)
{
	static t_window	*window = NULL;

	if (get_window)
		window = get_window;
	else
		return (window);
	return (NULL);
}

t_ui_state	*get_ui_state(t_ui_state *get_state)
{
	static t_ui_state	*state = NULL;

	if (get_state)
		state = get_state;
	else
		return (state);
	return (NULL);
}

int	edit_slider_var(float *unit, t_ui_state *state)
{
	int	x;
	int	y;

	SDL_GetMouseState(&x, &y);
	if (state->mouse_location == MOUSE_LOCATION_UI && state->m1_drag
		&& y >= state->ui_text_y_pos + 4 && y <= state->ui_text_y_pos + 15
		&& x < UI_SLIDER_WIDTH - 2 && x >= 2)
	{
		*unit = (float)(x - 2)
			/ (float)(UI_SLIDER_WIDTH - UI_SLIDER_BUTTON_WIDTH);
		return (TRUE);
	}
	return (FALSE);
}

int	edit_call_var(t_ui_state *state, t_rect rect)
{
	if (state->mouse_location == MOUSE_LOCATION_UI && state->m1_click
		&& mouse_collision(rect, state->mouse))
		return (TRUE);
	return (FALSE);
}
