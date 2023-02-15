/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	is_player_in_air(t_level *level, float height)
{
	float	dist;
	int		index;
	t_ray	r;

	r.pos.x = level->cam.pos.x;
	r.pos.y = level->cam.pos.y;
	r.pos.z = level->cam.pos.z;
	r.dir.x = 0;
	r.dir.y = 1;
	r.dir.z = 0;
	dist = cast_all(r, level, &index);
	if (dist < height + .02
		&& - level->all.tris[index].normal.y > WALKABLE_NORMAL_MIN_Y
		&& !level->ui.noclip)
	{
		return (FALSE);
	}
	else
		return (TRUE);
}

void	noclip(t_level *level, t_vec3 *wishdir, float delta_time)
{
	if (wishdir->x && wishdir->y && wishdir->z)
		vec_normalize(wishdir);
	vec_mult(wishdir, NOCLIP_SPEED);
	level->cam.pos.x += wishdir->x * delta_time;
	level->cam.pos.y += wishdir->y * delta_time;
	level->cam.pos.z += wishdir->z * delta_time;
	level->ui.horizontal_velocity = vec_length(*wishdir);
	level->game_logic.player.vel = (t_vec3){0, 0, 0};
}

void	rotate_wishdir(t_level *level, t_vec3 *wishdir)
{
	float	w;

	if (wishdir->x && wishdir->z)
	{
		w = sqrt(wishdir->x * wishdir->x + wishdir->z * wishdir->z);
		wishdir->x /= w;
		wishdir->z /= w;
	}
	rotate_vertex(level->cam.look_side, wishdir, 0);
}

int	vertical_movement(t_vec3 *wishdir, t_vec3 *vel, float delta_time,
																	int in_air)
{
	if (in_air)
	{
		wishdir->y = GRAVITY;
		vel->y += wishdir->y * delta_time;
	}
	else
	{
		if (vel->y > 0)
			vel->y = 0;
		vel->y = wishdir->y * JUMP_SPEED;
		if (wishdir->y == -1)
			return (1);
	}
	return (0);
}

void	air_movement(t_vec3 *wishdir, t_vec3 *vel, float delta_time)
{
	float	length;
	float	speed;
	float	addspeed;
	float	accelspeed;

	if (wishdir->x || wishdir->z)
	{
		length = sqrt(wishdir->x * wishdir->x + wishdir->z * wishdir->z);
		wishdir->x /= length;
		wishdir->z /= length;
		speed = fmax(vel->x * wishdir->x + vel->z * wishdir->z, 0);
		addspeed = 1 - speed;
		if (addspeed < 0)
			return ;
		accelspeed = AIR_ACCEL * delta_time;
		if (accelspeed > addspeed)
			accelspeed = addspeed;
		vel->x += accelspeed * wishdir->x;
		vel->z += accelspeed * wishdir->z;
	}
}
