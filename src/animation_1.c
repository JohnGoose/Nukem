/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	interpolate_tris(t_obj *target, t_obj_animation *animation,
						int node_amount, float time)
{
	float	node_time;
	float	elem_size;
	int		i;
	t_ivec2	id;

	id.x = ((int)(node_amount * time) + 0) % animation->keyframe_amount;
	id.y = ((int)(node_amount * time) + 1) % animation->keyframe_amount;
	elem_size = 1.0 / node_amount;
	node_time = (time - (elem_size * id.x)) / elem_size;
	i = 0;
	while (i < target->tri_amount)
	{
		tri_lerp(&target->tris[i], &animation->keyframes[id.x].tris[i],
			&animation->keyframes[id.y].tris[i], node_time);
		i++;
	}
}

void	play_animation(t_obj *target, t_obj_animation *animation,
						unsigned int start_time)
{
	float	time;
	int		node_amount;

	if (!animation->duration_multiplier)
		return ;
	time = (SDL_GetTicks() - start_time)
		/ (1000.0 * animation->duration / animation->duration_multiplier);
	node_amount = animation->keyframe_amount;
	if (start_time && !animation->loop)
	{
		node_amount--;
		if (time >= 1.0)
		{
			obj_copy(target,
				&animation->keyframes[animation->keyframe_amount - 1]);
			return ;
		}
	}
	else
		time = fmod(time, 1.0);
	interpolate_tris(target, animation, node_amount, time);
}
