/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_occlusion_0.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/09 15:05:47 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	cull_behind_occlusion(t_vec3 dir, t_vec3 pos, t_tri tri)
{
	t_vec3	vert;
	int		i;

	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&vert, tri.verts[i].pos, pos);
		if (vec_dot(dir, vert) >= 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	is_value_close(float a, float b, float range)
{
	float	diff;

	diff = a - b;
	return (-range <= diff && diff <= range);
}

float	area(t_vec3 *a, t_vec3 *b, t_vec3 *c)
{
	return (fabs(
			(a->x * (b->y - c->y) + b->x * (c->y - a->y) + c->x * (a->y - b->y))
			/ 2.0));
}

int	vec3_point_in_tri(t_vec3 *p, t_vec3 *a, t_vec3 *b, t_vec3 *c)
{
	float	a0;
	float	a1;
	float	a2;
	float	a3;

	a0 = area(a, b, c);
	a1 = area(p, b, c);
	a2 = area(a, p, c);
	a3 = area(a, b, p);
	return (is_value_close(
			a0, a1 + a2 + a3, OCCLUSION_CULLING_FLOAT_ERROR_MAGIC_NUMBER));
}

int	vec3_point_in_face(t_vec3 *p, t_tri *face)
{
	if (vec3_point_in_tri(p,
			&face->verts[0].pos, &face->verts[1].pos, &face->verts[2].pos))
		return (1);
	if (face->isquad
		&& vec3_point_in_tri(p,
			&face->verts[3].pos, &face->verts[2].pos, &face->verts[1].pos))
		return (1);
	return (0);
}
