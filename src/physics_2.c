/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	horizontal_movement_no_input(t_vec3 *vel, float delta_time)
{
	if (fabs(vel->x * GROUND_FRICTION * delta_time) > fabs(vel->x)
		|| fabs(vel->z * GROUND_FRICTION * delta_time) > fabs(vel->z))
	{
		vel->x = 0;
		vel->z = 0;
	}
	else
	{
		vel->x -= vel->x * GROUND_FRICTION * delta_time;
		vel->z -= vel->z * GROUND_FRICTION * delta_time;
	}
}

void	horizontal_movement(t_vec3 *wishdir, t_vec3 *vel,
								float delta_time, float movespeed)
{
	float	speed;

	if (wishdir->x || wishdir->z)
	{
		vel->x += wishdir->x * MOVE_ACCEL * delta_time;
		vel->z += wishdir->z * MOVE_ACCEL * delta_time;
		speed = sqrtf(vel->x * vel->x + vel->z * vel->z);
		if (speed > movespeed)
		{
			vel->x *= movespeed / speed;
			vel->z *= movespeed / speed;
		}
	}
	else
		horizontal_movement_no_input(vel, delta_time);
}

void	apply_velocity(t_vec3 vel, float h, t_level *level, float delta_time)
{
	if (vel.x || vel.y || vel.z)
	{
		vec_mult(&vel, delta_time);
		while (player_collision(&vel, &level->cam.pos, level, h))
			;
		vec_add(&level->cam.pos, level->cam.pos, vel);
		vec_div(&vel, delta_time);
		level->ui.horizontal_velocity = sqrt(vel.x * vel.x + vel.z * vel.z);
	}
	level->game_logic.player.vel = vel;
}

void	movement_physics(t_level *level, float delta_time,
								t_player *player)
{
	t_vec3	vel;
	int		in_air;

	vel = player->vel;
	in_air = is_player_in_air(level, player->height);
	vertical_movement(&player->wishdir, &vel, delta_time, in_air);
	if (in_air || player->wishdir.y)
		air_movement(&player->wishdir, &vel, delta_time);
	else
		horizontal_movement(&player->wishdir, &vel, delta_time,
			player->move_speed);
	apply_velocity(vel, player->height, level, delta_time);
}

void	player_movement(t_level *level)
{
	float		delta_time;
	t_player	*player;

	player = &level->game_logic.player;
	delta_time = level->ui.frame_time / 1000.;
	player->height = PLAYER_EYE_HEIGHT;
	player_input(level, player);
	rotate_wishdir(level, &player->wishdir);
	if (level->ui.noclip)
		return (noclip(level, &player->wishdir, delta_time));
	movement_physics(level, delta_time, player);
	if (level->ui.physics_debug)
	{
		level->ui.wishdir.x = player->wishdir.x;
		level->ui.wishdir.y = player->wishdir.z;
	}
}
