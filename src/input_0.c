/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	typing_input_backspace(t_level *level)
{
	int	i;

	i = 0;
	while (level->ui.state.save_filename[i])
		i++;
	if (!i)
		level->ui.state.text_input_enable = FALSE;
	else
	{
		i--;
		level->ui.state.save_filename[i] = '\0';
	}
}

void	typing_input(t_level *level, SDL_Event event)
{
	int		i;
	int		k;
	char	c;

	if (event.type == SDL_TEXTINPUT)
	{
		i = 0;
		while (i < SDL_TEXTINPUTEVENT_TEXT_SIZE)
		{
			c = event.text.text[i];
			if (c < ' ' || c > '~')
				break ;
			k = 0;
			while (level->ui.state.save_filename[k])
				k++;
			level->ui.state.save_filename[k] = c;
			k++;
			level->ui.state.save_filename[k] = '\0';
			i++;
		}
	}
	else if (event.type == SDL_KEYDOWN
		&& event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
		typing_input_backspace(level);
}

void	set_mouse_input_location_3d_space(t_level *level)
{
	gizmo(level);
	if (level->ui.state.mouse_location != MOUSE_LOCATION_GIZMO_X
		&& level->ui.state.mouse_location != MOUSE_LOCATION_GIZMO_Y
		&& level->ui.state.mouse_location != MOUSE_LOCATION_GIZMO_Z)
	{
		if (level->ui.state.ui_location == UI_LOCATION_LIGHT_EDITOR)
		{
			level->ui.state.mouse_location = MOUSE_LOCATION_LIGHT_EDITOR;
			if (level->ui.state.m1_click && level->bake_status != BAKE_BAKING)
				select_light(level);
		}
		else if (level->ui.state.ui_location == UI_LOCATION_GAME_SETTINGS)
		{
			level->ui.state.mouse_location = MOUSE_LOCATION_GAME_SETTINGS;
			if (level->ui.state.m1_click)
				game_logic_select_nearest_to_mouse(level);
		}
		else
		{
			level->ui.state.mouse_location = MOUSE_LOCATION_SELECTION;
			select_face(&level->cam, level);
		}
		level->ui.state.m1_drag = FALSE;
	}
}

void	get_mouse_coordinate(t_level *level, int *x, int *y)
{
	if (level->ui.state.mouse_capture)
	{
		*x = RES_X / 2;
		*y = RES_Y / 2;
	}
	else
	{
		SDL_GetMouseState(x, y);
		level->ui.state.mouse.x = *x;
		level->ui.state.mouse.y = *y;
	}
}

void	set_mouse_input_location(t_level *level, t_game_state game_state)
{
	int	x;
	int	y;

	get_mouse_coordinate(level, &x, &y);
	if (game_state == GAME_STATE_MAIN_MENU)
	{
		if (level->ui.main_menu == MAIN_MENU_LOCATION_MAIN)
			level->ui.state.mouse_location = MOUSE_LOCATION_MAIN_MENU;
		else
			level->ui.state.mouse_location = MOUSE_LOCATION_UI;
	}
	else if (level->ui.state.mouse_capture || game_state == GAME_STATE_INGAME)
		level->ui.state.mouse_location = MOUSE_LOCATION_GAME;
	else if (x < level->ui.state.ui_max_width
		&& y < level->ui.state.ui_text_y_pos)
		level->ui.state.mouse_location = MOUSE_LOCATION_UI;
	else if (level->ui.state.ui_location == UI_LOCATION_UV_EDITOR
		&& x < RES_X / 2)
		level->ui.state.mouse_location = MOUSE_LOCATION_UV_EDITOR;
	else
		set_mouse_input_location_3d_space(level);
	gizmo(level);
	level->ui.state.m1_click = FALSE;
}
