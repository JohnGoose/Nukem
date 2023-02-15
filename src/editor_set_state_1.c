/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_set_state_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_win_pos(t_level *level)
{
	level->game_logic.win_pos = level->cam.pos;
}

void	set_spawn_pos(t_level *level)
{
	level->game_logic.spawn_pos.pos = level->cam.pos;
	level->game_logic.spawn_pos.look_side = level->cam.look_side;
	level->game_logic.spawn_pos.look_up = level->cam.look_up;
}
