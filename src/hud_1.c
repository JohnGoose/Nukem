/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	hud(t_level *level, unsigned int *pixels)
{
	if (level->game_logic.death_start_time)
	{
		level->ui.state.current_font = level->ui.win_lose_font;
		death_overlay(pixels);
		set_text_color(HUD_GAME_EVENT_TEXT_COLOR);
		render_text("DEAD",
			RES_X / 2 - HUD_GAME_EVENT_FONT_SIZE,
			RES_Y / 2 - HUD_GAME_EVENT_FONT_SIZE / 2);
	}
	else if (level->game_logic.win_start_time)
	{
		level->ui.state.current_font = level->ui.win_lose_font;
		set_text_color(cycle_rgb(SDL_GetTicks()));
		render_text("YOU WIN",
			RES_X / 2 - HUD_GAME_EVENT_FONT_SIZE * 2,
			RES_Y / 2 - HUD_GAME_EVENT_FONT_SIZE / 2);
	}
	else
		ingame_hud(level, pixels);
}
