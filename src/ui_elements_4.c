/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_elements_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	file_browser(char *str, char *extension, void (*f)(t_level *, char *))
{
	t_ui_state	*state;

	state = get_ui_state(NULL);
	if (call(str, NULL))
	{
		state->ui_location = UI_LOCATION_FILE_OPEN;
		ft_strcpy(state->extension, extension);
		state->open_file = *f;
	}
}

void	text_input(char *str, t_level *level)
{
	char	*filename;

	if (!str[0])
	{
		if (call("input:     ", NULL))
			level->ui.state.text_input_enable = TRUE;
		return ;
	}
	filename = ft_strjoin(str, ".doom-nukem");
	if (call(filename, NULL))
		level->ui.state.text_input_enable = TRUE;
	free(filename);
}

int	call(char *str, void (*f)(t_level *))
{
	t_ui_state	*state;
	t_level		*level;
	t_ivec2		size;
	t_rect		rect;
	int			tmp;

	state = get_ui_state(NULL);
	level = get_level(NULL);
	state->ui_text_x_offset = 4;
	tmp = state->ui_text_color;
	state->ui_text_color = UI_BACKGROUND_COL;
	size = render_text(str, 4, state->ui_text_y_pos);
	state->ui_text_color = tmp;
	if (state->ui_max_width < state->ui_text_x_offset + size.x)
		state->ui_max_width = state->ui_text_x_offset + size.x;
	rect = render_call(&size, state);
	tmp = edit_call_var(state, rect);
	if (tmp && *f)
		(*f)(level);
	state->ui_text_y_pos += UI_ELEMENT_HEIGHT;
	if (tmp)
		level->ui.state.m1_drag = FALSE;
	return (tmp);
}
