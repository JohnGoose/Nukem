/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d_intersect_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	check_uv_edge(int i, int j, t_tri t1, t_tri t2)
{
	int		res;

	res = 0;
	if (i == 2 + t1.isquad)
	{
		if (j == 2 + t2.isquad)
			res = line_intersect(t1.verts[i].txtr, t1.verts[0].txtr,
					t2.verts[j].txtr, t2.verts[0].txtr);
		else
			res = line_intersect(t1.verts[i].txtr, t1.verts[0].txtr,
					t2.verts[j].txtr, t2.verts[j + 1].txtr);
	}
	else
	{
		if (j == 2 + t2.isquad)
			res = line_intersect(t1.verts[i].txtr, t1.verts[i + 1].txtr,
					t2.verts[j].txtr, t2.verts[0].txtr);
		else
			res = line_intersect(t1.verts[i].txtr, t1.verts[i + 1].txtr,
					t2.verts[j].txtr, t2.verts[j + 1].txtr);
	}
	return (res);
}

int	tri_uv_intersect(t_tri t1, t_tri t2)
{
	int	i;
	int	j;

	i = 0;
	if (t1.isquad)
		calc_quad_uv(&t1);
	if (t2.isquad)
		calc_quad_uv(&t2);
	while (i < 3 + t1.isquad)
	{
		if (point_in_tri(t1.verts[i].txtr, t2.verts[0].txtr,
				t2.verts[1].txtr, t2.verts[2].txtr) || (t2.isquad
				&& point_in_tri(t1.verts[i].txtr, t2.verts[3].txtr,
					t2.verts[1].txtr, t2.verts[2].txtr)))
			return (1);
		j = 0;
		while (j < 3 + t2.isquad)
		{
			if (check_uv_edge(i, j, t1, t2))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
