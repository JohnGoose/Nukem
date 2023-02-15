/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_occlusion_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	is_bface_in_aface(t_tri a, t_tri b, t_level *level)
{
	int	vert_amount;
	int	i;

	vert_amount = 3 + a.isquad;
	i = -1;
	while (++i < vert_amount)
	{
		camera_offset(&a.verts[i].pos, &level->cam);
		if (a.verts[i].pos.z < 0)
			return (0);
	}
	vert_amount = 3 + b.isquad;
	i = -1;
	while (++i < vert_amount)
	{
		camera_offset(&b.verts[i].pos, &level->cam);
		if (b.verts[i].pos.z < 0)
			return (0);
	}
	i = -1;
	while (++i < vert_amount)
		if (!vec3_point_in_face(&b.verts[i].pos, &a))
			return (0);
	return (1);
}

int	occlusion_culling(t_tri tri, t_level *level)
{
	int	i;

	i = 0;
	while (i < level->visible.tri_amount)
	{
		if (!level->visible.tris[i].opacity)
			if (is_bface_in_aface(level->visible.tris[i], tri, level))
				if (!cull_behind_occlusion(level->visible.tris[i].normal,
						level->visible.tris[i].verts[0].pos, tri))
					return (0);
		i++;
	}
	return (1);
}
