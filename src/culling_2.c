/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	vertex_directions(t_vec3 corner[4], t_level *l, int i, t_vec3 pos)
{
	corner[0] = l->all.tris[i].verts[0].pos;
	corner[1] = l->all.tris[i].verts[1].pos;
	corner[2] = l->all.tris[i].verts[2].pos;
	corner[3] = l->all.tris[i].verts[3].pos;
	vec_sub(&corner[0], pos, corner[0]);
	vec_sub(&corner[1], pos, corner[1]);
	vec_sub(&corner[2], pos, corner[2]);
	vec_sub(&corner[3], pos, corner[3]);
	vec_normalize(&corner[0]);
	vec_normalize(&corner[1]);
	vec_normalize(&corner[2]);
	vec_normalize(&corner[3]);
}

t_vec3	camera_to_reflection(t_level *level, int i)
{
	t_vec3	normal;
	t_vec3	pos;

	memset(&pos, 0, sizeof(t_vec3));
	level->all.tris[i].reflection_obj_first_bounce.tri_amount = 0;
	vec_sub(&pos, level->all.tris[i].verts[0].pos, level->cam.pos);
	normal = level->all.tris[i].normal;
	vec_mult(&normal, vec_dot(pos, normal));
	vec_add(&pos, level->cam.pos, normal);
	vec_add(&pos, pos, normal);
	return (pos);
}

void	reflection_culling_first_bounce(t_level *level, int i)
{
	t_vec3	pos;
	t_vec3	corner[4];
	t_vec3	side_normals[4];
	int		k;

	pos = camera_to_reflection(level, i);
	vertex_directions(corner, level, i, pos);
	calculate_side_normals(side_normals, corner);
	if (!level->all.tris[i].isquad)
		side_normals[3] = side_normals[0];
	k = -1;
	while (++k < level->all.tris[i].reflection_obj_all.tri_amount)
		if ((!level->ui.distance_culling
				|| distance_culling(
					level->all.tris[i].reflection_obj_all.tris[k],
					level->cam.pos, level->ui.render_distance))
			&& (level->all.tris[i].reflection_obj_all.tris[k].isgrid
				|| frustrum_culling(side_normals, pos,
					level->all.tris[i].reflection_obj_all.tris[k])))
			level->all.tris[i].reflection_obj_first_bounce.tris[
				level->all.tris[i].reflection_obj_first_bounce.tri_amount++]
				= level->all.tris[i].reflection_obj_all.tris[k];
}

int	face_distance_culling(t_tri t1, t_tri t2, t_level *level)
{
	int	i;

	i = 0;
	if (!level->ui.distance_culling)
		return (1);
	while (i < 3 + t1.isquad)
	{
		if (distance_culling(t2, t1.verts[i].pos, level->ui.render_distance))
			return (1);
		i++;
	}
	return (0);
}

void	reflection_culling(t_level *level, int i)
{
	t_vec3	avg;
	int		amount;
	int		k;

	level->all.tris[i].reflection_obj_all.tri_amount = 0;
	memset(&avg, 0, sizeof(t_vec3));
	k = -1;
	while (++k < 3 + level->all.tris[i].isquad)
		vec_add(&avg, avg, level->all.tris[i].verts[k].pos);
	vec_div(&avg, 3 + level->all.tris[i].isquad);
	amount = 0;
	k = -1;
	while (++k < level->all.tri_amount)
	{
		if ((level->all.tris[k].isgrid
				|| cull_behind(level->all.tris[i].normal,
					avg, level->all.tris[k]))
			&& face_distance_culling(level->all.tris[i],
				level->all.tris[k], level)
			&& reflection_backface(level->all.tris[k], level->all.tris[i]))
			level->all.tris[i].reflection_obj_all.tris[amount++]
				= level->all.tris[k];
	}
	level->all.tris[i].reflection_obj_all.tri_amount = amount;
}
