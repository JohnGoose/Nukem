/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	backface_culling(t_vec3 pos, t_tri tri)
{
	t_vec3	normal;
	t_vec3	diff;

	vec_cross(&normal, tri.v0v1, tri.v0v2);
	vec_sub(&diff, tri.verts[0].pos, pos);
	if (vec_dot(diff, normal) > 0)
		return (1);
	vec_sub(&diff, tri.verts[1].pos, pos);
	if (vec_dot(diff, normal) > 0)
		return (1);
	vec_sub(&diff, tri.verts[2].pos, pos);
	if (vec_dot(diff, normal) > 0)
		return (1);
	if (tri.isquad)
	{
		vec_sub(&diff, tri.verts[3].pos, pos);
		if (vec_dot(diff, normal) > 0)
			return (1);
	}
	return (0);
}

int	reflection_backface(t_tri t1, t_tri t2)
{
	int	i;

	i = 0;
	while (i < 3 + t2.isquad)
	{
		if (backface_culling(t2.verts[i].pos, t1))
			return (1);
		i++;
	}
	return (0);
}

void	calculate_side_normals(t_vec3 normal[4], t_vec3 corner[4])
{
	vec_cross(&normal[0], corner[2], corner[0]);
	vec_cross(&normal[1], corner[1], corner[3]);
	vec_cross(&normal[2], corner[0], corner[1]);
	vec_cross(&normal[3], corner[3], corner[2]);
}

void	face_border_normals(t_vec3 v[4], t_vec3 dir, t_tri target)
{
	t_vec3	tmp;

	vec_sub(&tmp, target.verts[0].pos, target.verts[1].pos);
	vec_cross(&v[0], tmp, dir);
	vec_sub(&tmp, target.verts[2].pos, target.verts[0].pos);
	vec_cross(&v[2], tmp, dir);
	if (target.isquad)
	{
		vec_sub(&tmp, target.verts[1].pos, target.verts[3].pos);
		vec_cross(&v[1], tmp, dir);
		vec_sub(&tmp, target.verts[3].pos, target.verts[2].pos);
		vec_cross(&v[3], tmp, dir);
	}
	else
	{
		vec_sub(&tmp, target.verts[1].pos, target.verts[2].pos);
		vec_cross(&v[1], tmp, dir);
	}
}

void	shadow_face_culling(t_level *level, int i)
{
	t_tri	target;
	t_vec3	v[4];
	int		k;

	target = level->all.tris[i];
	face_border_normals(v, level->ui.sun_dir, target);
	k = -1;
	level->all.tris[i].shadow_faces.tri_amount = 0;
	while (++k < level->all.tri_amount)
		if (k != i && cull_behind(level->all.tris[i].normal,
				target.verts[0].pos, level->all.tris[k])
			&& (level->all.tris[k].isgrid || target.isgrid
				|| (cull_behind(v[0], target.verts[0].pos, level->all.tris[k])
					&& cull_behind(v[1], target.verts[1].pos,
						level->all.tris[k])
					&& cull_behind(v[2], target.verts[2].pos,
						level->all.tris[k])
					&& (!level->all.tris[i].isquad
						|| cull_behind(v[3], target.verts[3].pos,
							level->all.tris[k])))))
			level->all.tris[i].shadow_faces.tris[
				level->all.tris[i].shadow_faces.tri_amount++]
				= level->all.tris[k];
}
