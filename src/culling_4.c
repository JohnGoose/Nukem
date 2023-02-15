/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/06 00:07:23 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	static_culling(t_level *l)
{
	int	i;

	i = 0;
	while (i < l->all.tri_amount)
	{
		if (l->all.tris[i].reflectivity)
			reflection_culling(l, l->all.tris[i].index);
		shadow_face_culling(l, l->all.tris[i].index);
		if (l->all.tris[i].opacity
			&& l->all.tris[i].opacity_precise
			&& l->all.tris[i].refractivity < 0)
			opacity_culling(l, l->all.tris[i].index, &l->all);
		i++;
	}
}

void	cull_visible(t_level *l, t_vec3 side_normals[4])
{
	int	i;

	l->visible.tri_amount = 0;
	visible_request_merge(l, l->all.tri_amount);
	i = -1;
	while (++i < l->all.tri_amount)
	{
		if (l->all.tris[i].isgrid
			|| (cull_behind(l->cam.front, l->cam.pos, l->all.tris[i])
				&& frustrum_culling(side_normals, l->cam.pos, l->all.tris[i])
				&& (!l->ui.distance_culling
					|| distance_culling(l->all.tris[i], l->cam.pos,
						l->ui.render_distance))
				&& (!l->ui.backface_culling
					|| backface_culling(l->cam.pos, l->all.tris[i]))
				&& (!l->ui.occlusion_culling || !l->ui.backface_culling
					|| l->all.tris[i].opacity
					|| occlusion_culling(l->all.tris[i], l))))
			l->visible.tris[l->visible.tri_amount++] = l->all.tris[i];
	}
}

void	culling(t_level *l)
{
	t_vec3		corner[4];
	t_vec3		side_normals[4];
	int			i;

	calculate_corner_vectors(corner, &l->cam);
	calculate_side_normals(side_normals, corner);
	skybox_culling(l, &l->cam, side_normals);
	cull_visible(l, side_normals);
	i = -1;
	while (++i < l->visible.tri_amount)
	{
		if (l->visible.tris[i].reflectivity)
			reflection_culling_first_bounce(l, l->visible.tris[i].index);
		if (l->visible.tris[i].opacity_precise
			&& l->visible.tris[i].refractivity > 0)
			opacity_culling(l, l->visible.tris[i].index, &l->visible);
	}
}
