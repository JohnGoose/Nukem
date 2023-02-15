/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_space_partition_0.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	get_ssp(t_ivec2 pixel)
{
	int	x;
	int	y;

	x = (float)pixel.x / RES_X * SSP_MAX_X;
	y = (float)pixel.y / RES_Y * SSP_MAX_Y;
	return (x + y * SSP_MAX_X);
}

int	get_ssp_coordinate(int coord, int horizontal)
{
	if (horizontal)
		return ((int)((float)coord / RES_X * SSP_MAX_X));
	return ((int)((float)coord / RES_Y * SSP_MAX_Y));
}

void	ssp_edge_vectors(t_vec3 result[2], t_camera c,
											float px, int horizontal)
{
	float	ym;
	float	xm;

	if (horizontal)
		ym = c.fov_y / RES_Y * RES_Y - c.fov_y / 2;
	if (horizontal)
		xm = c.fov_x / RES_X * px - c.fov_x / 2;
	if (!horizontal)
		ym = c.fov_y / RES_Y * px - c.fov_y / 2;
	if (!horizontal)
		xm = c.fov_x / RES_X * RES_X - c.fov_x / 2;
	result[0].x = c.front.x + c.up.x * ym + c.side.x * xm;
	result[0].y = c.front.y + c.up.y * ym + c.side.y * xm;
	result[0].z = c.front.z + c.up.z * ym + c.side.z * xm;
	if (horizontal)
		ym = c.fov_y / RES_Y * 0 - c.fov_y / 2;
	if (horizontal)
		xm = c.fov_x / RES_X * px - c.fov_x / 2;
	if (!horizontal)
		ym = c.fov_y / RES_Y * px - c.fov_y / 2;
	if (!horizontal)
		xm = c.fov_x / RES_X * 0 - c.fov_x / 2;
	result[1].x = c.front.x + c.up.x * ym + c.side.x * xm;
	result[1].y = c.front.y + c.up.y * ym + c.side.y * xm;
	result[1].z = c.front.z + c.up.z * ym + c.side.z * xm;
}

int	horizontal(t_tri tri, int x, t_camera cam, int side)
{
	t_vec3	corners[2];
	t_vec3	normal;
	t_vec3	vert;
	int		i;

	ssp_edge_vectors(corners, cam, (float)x, 1);
	vec_cross(&normal, corners[side], corners[!side]);
	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&vert, tri.verts[i].pos, cam.pos);
		if (vec_dot(vert, normal) > 0)
			return (0);
		i++;
	}
	return (1);
}

int	vertical(t_tri tri, int x, t_camera cam, int side)
{
	t_vec3	corners[2];
	t_vec3	normal;
	t_vec3	vert;
	int		i;

	ssp_edge_vectors(corners, cam, (float)x, 0);
	vec_cross(&normal, corners[!side], corners[side]);
	i = 0;
	while (i < 3 + tri.isquad)
	{
		vec_sub(&vert, tri.verts[i].pos, cam.pos);
		if (vec_dot(vert, normal) > 0)
			return (0);
		i++;
	}
	return (1);
}
