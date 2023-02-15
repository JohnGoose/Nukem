/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/05 23:30:05 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	game_logic_win_lose(t_level *level, t_game_state *game_state)
{
	t_vec3	dist;

	if (level->game_logic.player.health <= 0)
	{
		level->game_logic.reload_start_time = 0;
		level->game_logic.death_start_time = SDL_GetTicks();
		level->cam.look_up = 1;
	}
	else if (*game_state == GAME_STATE_INGAME)
	{
		vec_sub(&dist, level->game_logic.win_pos, level->cam.pos);
		if (vec_length(dist) < level->game_logic.win_dist)
			level->game_logic.win_start_time = SDL_GetTicks();
	}
}

void	game_logic(t_level *level, t_game_state *game_state)
{
	handle_audio(level);
	if (level->game_logic.win_start_time || level->game_logic.death_start_time)
		game_finished(level, game_state);
	if (level->game_logic.death_start_time)
		return ;
	if (level->game_logic.reload_start_time)
		player_reload(level);
	else if (level->ui.state.m1_click && level->ui.state.mouse_capture)
		player_shoot(level);
	if (level->game_logic.win_start_time)
		return ;
	if (*game_state == GAME_STATE_INGAME)
	{
		if (pick_up_pick_ups(level, level->game_logic.ammo_box,
				level->game_logic.ammo_box_amount))
			level->game_logic.player.ammo = PLAYER_AMMO_MAX;
		if (pick_up_pick_ups(level, level->game_logic.health_box,
				level->game_logic.health_box_amount))
			level->game_logic.player.health = PLAYER_HEALTH_MAX;
	}
	else
		reset_pick_ups(level);
	game_logic_win_lose(level, game_state);
}
