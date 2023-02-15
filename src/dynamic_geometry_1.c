/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_geometry_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 14:54:44 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	merge_game_models(t_level *level, t_game_state game_state)
{
	static float	rot = 0;
	int				i;

	rot += level->game_logic.item_spin_speed;
	if ((game_state == GAME_STATE_EDITOR
			&& level->ui.state.ui_location == UI_LOCATION_GAME_SETTINGS)
		|| game_state == GAME_STATE_INGAME)
	{
		merge_pickups(level, rot);
		if (game_state == GAME_STATE_INGAME)
			merge_enemies_runtime(level);
		else
			merge_enemies_editor(level, rot);
	}
	i = -1;
	while (++i < level->game_logic.projectile_amount)
		merge_sprite(level, level->game_logic.projectiles[i].pos,
			&level->game_models.projectile_sprite);
	if (game_state != GAME_STATE_EDITOR)
		return ;
	i = -1;
	while (++i < level->light_amount)
		merge_sprite(level, level->lights[i].pos,
			&level->game_models.light_sprite);
}

t_vec2	viewmodel_sway(t_level *level)
{
	static t_vec2	delta_vel = {0, 0};
	t_vec2			res;

	delta_vel.x -= level->cam.look_side;
	delta_vel.y -= level->cam.look_up;
	delta_vel.x /= .08 * (float)level->ui.frame_time;
	delta_vel.y /= .08 * (float)level->ui.frame_time;
	delta_vel.x = clamp(delta_vel.x, -0.15, 0.15);
	delta_vel.y = clamp(delta_vel.y, -0.6, 0.6);
	res = delta_vel;
	delta_vel.x = level->cam.look_side;
	delta_vel.y = level->cam.look_up;
	return (res);
}

void	merge_viewmodel(t_level *level)
{
	t_vec2	sway;

	sway = viewmodel_sway(level);
	merge_mesh(level, &level->game_models.viewmodel, level->cam.pos,
		(t_vec2){level->cam.look_up + sway.y, level->cam.look_side + sway.x});
}
