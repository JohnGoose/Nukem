/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 20:19:32 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/09 20:07:26 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	physics_raycast(t_ray r, t_level *level, t_vec3 *vel)
{
	float	dist;
	int		index;
	t_vec3	normal;

	dist = cast_all(r, level, &index);
	if (dist != FLT_MAX)
	{
		normal = level->all.tris[index].normal;
		if ((normal.y < 0 || - normal.y < WALKABLE_NORMAL_MIN_Y)
			&& dist <= vec_length(*vel) + WALL_CLIP_DIST)
		{
			vec_mult(&normal, vec_dot(*vel, normal));
			vec_sub(vel, *vel, normal);
			return (1);
		}
	}
	return (0);
}
