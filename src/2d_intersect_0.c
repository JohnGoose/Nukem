/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d_intersect_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/09 14:58:06 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	sign(t_vec2 p1, t_vec2 p2, t_vec2 p3)
{
	return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

int	point_in_tri(t_vec2 pt, t_vec2 v1, t_vec2 v2, t_vec2 v3)
{
	int		has_neg;
	int		has_pos;
	float	d1;
	float	d2;
	float	d3;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);
	has_neg = ((d1 < 0) || (d2 < 0) || (d3 < 0));
	has_pos = ((d1 > 0) || (d2 > 0) || (d3 > 0));
	return (!(has_neg && has_pos));
}

int	line_orientation(t_vec2 p, t_vec2 q, t_vec2 r)
{
	float	val;

	val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0)
		return (0);
	if (val > 0)
		return (1);
	else
		return (2);
}

int	line_intersect(t_vec2 p1, t_vec2 q1, t_vec2 p2, t_vec2 q2)
{
	int	o1;
	int	o2;
	int	o3;
	int	o4;

	o1 = line_orientation(p1, q1, p2);
	o2 = line_orientation(p1, q1, q2);
	o3 = line_orientation(p2, q2, p1);
	o4 = line_orientation(p2, q2, q1);
	if (o1 != o2 && o3 != o4)
		return (TRUE);
	return (FALSE);
}

void	calc_quad_uv(t_tri *tri)
{
	tri->verts[3].txtr.x
		= tri->verts[2].txtr.x + (tri->verts[1].txtr.x - tri->verts[0].txtr.x);
	tri->verts[3].txtr.y
		= tri->verts[2].txtr.y + (tri->verts[1].txtr.y - tri->verts[0].txtr.y);
}
