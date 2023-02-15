/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	texture_minmax(t_vec2 *min, t_vec2 *max, t_tri tri)
{
	min->x = tri.verts[0].txtr.x;
	min->x = fmin(tri.verts[1].txtr.x, min->x);
	min->x = fmin(tri.verts[2].txtr.x, min->x);
	min->y = 1 - tri.verts[0].txtr.y;
	min->y = fmin(1 - tri.verts[1].txtr.y, min->y);
	min->y = fmin(1 - tri.verts[2].txtr.y, min->y);
	max->x = tri.verts[0].txtr.x;
	max->x = fmax(tri.verts[1].txtr.x, max->x);
	max->x = fmax(tri.verts[2].txtr.x, max->x);
	max->y = 1 - tri.verts[0].txtr.y;
	max->y = fmax(1 - tri.verts[1].txtr.y, max->y);
	max->y = fmax(1 - tri.verts[2].txtr.y, max->y);
}

void	normalize_data(t_vec3 *uvw)
{
	vec_div(uvw, uvw->x + uvw->y + uvw->z);
}

void	get_uv(t_vec3 *uvw, t_ivec2 ipoint, t_tri tri, t_bmp *txtr)
{
	t_vec2	fp;
	t_vec2	v0;
	t_vec2	v1;
	t_vec2	v2;

	fp.x = (float)ipoint.x / txtr->width;
	fp.y = 1 - (float)ipoint.y / txtr->height;
	if (!point_in_tri(fp, tri.verts[0].txtr,
			tri.verts[1].txtr, tri.verts[2].txtr))
		v0 = tri.verts[3].txtr;
	else
		v0 = tri.verts[0].txtr;
	v1 = tri.verts[1].txtr;
	v2 = tri.verts[2].txtr;
	uvw->z = fabs((fp.x * (v1.y - v2.y)
				+ v1.x * (v2.y - fp.y)
				+ v2.x * (fp.y - v1.y)));
	uvw->y = fabs((v0.x * (fp.y - v2.y)
				+ fp.x * (v2.y - v0.y)
				+ v2.x * (v0.y - fp.y)));
	uvw->x = fabs((v0.x * (v1.y - fp.y)
				+ v1.x * (fp.y - v0.y)
				+ fp.x * (v0.y - v1.y)));
	normalize_data(uvw);
}

t_vec3	uv_to_3d(t_tri tri, t_bmp *txtr, t_ivec2 point)
{
	t_vec3	uvw;
	t_vec3	av0;
	t_vec3	av1;
	t_vec3	av2;
	t_vec2	fp;

	get_uv(&uvw, point, tri, txtr);
	fp.x = (float)point.x / txtr->width;
	fp.y = 1 - (float)point.y / txtr->height;
	if (!point_in_tri(fp, tri.verts[0].txtr,
			tri.verts[1].txtr, tri.verts[2].txtr))
		av0 = tri.verts[3].pos;
	else
		av0 = tri.verts[0].pos;
	av1 = tri.verts[1].pos;
	av2 = tri.verts[2].pos;
	vec_mult(&av0, uvw.z);
	vec_mult(&av1, uvw.y);
	vec_mult(&av2, uvw.x);
	vec_add(&av0, av0, av1);
	vec_add(&av0, av0, av2);
	return (av0);
}

void	wrap_coords(int *x, int *y, int max_x, int max_y)
{
	if (*y < 0)
		*y = max_y - (*y % max_y);
	if (*y >= max_y)
		*y = *y % max_y;
	if (*x < 0)
		*x = max_x - (*x % max_x);
	if (*x >= max_x)
		*x = *x % max_x;
}