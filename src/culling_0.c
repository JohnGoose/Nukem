/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	cull_ahead(t_vec3 dir, t_vec3 pos, t_tri tri)
{
	t_vec3	vert;
	int		i;

	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&vert, tri.verts[i].pos, pos);
		if (vec_dot(dir, vert) < 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	cull_behind(t_vec3 dir, t_vec3 pos, t_tri tri)
{
	t_vec3	vert;
	int		i;

	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&vert, tri.verts[i].pos, pos);
		if (vec_dot(dir, vert) > 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	frustrum_culling(t_vec3 side_normal[4], t_vec3 pos, t_tri tri)
{
	t_vec3	end;
	int		out[4][4];
	int		i;

	out[3][0] = 1;
	out[3][1] = 1;
	out[3][2] = 1;
	out[3][3] = 1;
	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&end, tri.verts[i].pos, pos);
		out[i][0] = vec_dot(side_normal[0], end) < 0;
		out[i][1] = vec_dot(side_normal[1], end) < 0;
		out[i][2] = vec_dot(side_normal[2], end) < 0;
		out[i][3] = vec_dot(side_normal[3], end) < 0;
		if (!out[i][0] && !out[i][1] && !out[i][2] && !out[i][3])
			return (1);
		i++;
	}
	i = -1;
	while (++i < 4)
		if (out[0][i] && out[1][i] && out[2][i] && out[3][i])
			return (0);
	return (1);
}

void	distance_minmax(float *min, float *max, t_tri tri, t_vec3 pos)
{
	t_vec3	diff;
	float	tmp;
	int		i;

	i = 0;
	*min = FLT_MAX;
	*max = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&diff, tri.verts[i].pos, pos);
		tmp = vec_length(diff);
		if (tmp < *min)
			*min = tmp;
		if (tmp > *max)
			*max = tmp;
		i++;
	}
}

int	distance_culling(t_tri tri, t_vec3 player, float render_distance)
{
	float	min;
	float	max;
	float	len;
	t_vec3	tmp_vec;

	distance_minmax(&min, &max, tri, player);
	if (min < render_distance)
		return (1);
	vec_sub(&tmp_vec, tri.verts[2].pos, tri.verts[1].pos);
	min = vec_length(tmp_vec);
	len = vec_length(tri.v0v1);
	if (len < min)
		len = min;
	min = vec_length(tri.v0v2);
	if (len < min)
		len = min;
	if (tri.isquad)
	{
		vec_sub(&tmp_vec, tri.verts[3].pos, tri.verts[0].pos);
		min = vec_length(tmp_vec);
		if (len < min)
			len = min;
	}
	return (max < len);
}
