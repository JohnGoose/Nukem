/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_space_partition_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	partition_range(int (*side_comparison)(t_tri, int, t_camera, int),
							t_tri tri, t_camera cam, float bounds[3])
{
	float	tmp;
	float	min;
	float	max;
	int		j;

	min = bounds[0];
	max = bounds[1];
	tmp = max;
	j = tri.index - 1;
	while (++j < bounds[2])
		if (side_comparison(tri, (min + tmp) / 2, cam, 1))
			min = (min + tmp) / 2;
	else
		tmp = (min + tmp) / 2;
	tmp = min;
	j = tri.index - 1;
	while (++j < bounds[2])
		if (side_comparison(tri, (max + tmp) / 2, cam, 0))
			max = (max + tmp) / 2;
	else
		tmp = (max + tmp) / 2;
	bounds[0] = min;
	bounds[1] = max;
}

void	find_partition(int (*side_comparison)(t_tri, int, t_camera, int),
							t_tri tri, t_camera cam, float bounds[3])
{
	float	min;
	float	max;
	int		i;

	min = bounds[0];
	max = bounds[1];
	i = 0;
	while (i < bounds[2])
	{
		if (side_comparison(tri, (max + min) / 2, cam, 0))
			max = (max + min) / 2;
		else if (side_comparison(tri, (max + min) / 2, cam, 1))
			min = (max + min) / 2;
		else
		{
			tri.index = i;
			bounds[0] = min;
			bounds[1] = max;
			partition_range(side_comparison, tri, cam, bounds);
			return ;
		}
		i++;
	}
	bounds[0] = min;
	bounds[1] = max;
}

void	ssp_set_tri(t_tri tri, t_level *level, int x, int y)
{
	if (level->ssp[x + y * SSP_MAX_X].tri_amount + 1
		>= level->ssp_max[x + y * SSP_MAX_X])
	{
		level->ssp[x + y * SSP_MAX_X].tris
			= (t_tri *)ft_realloc(level->ssp[x + y * SSP_MAX_X].tris,
				sizeof(t_tri) * level->ssp_max[x + y * SSP_MAX_X],
				sizeof(t_tri) * (int)(level->ssp_max[x + y * SSP_MAX_X] * 1.5));
		level->ssp_max[x + y * SSP_MAX_X] *= 1.5;
		if (!level->ssp[x + y * SSP_MAX_X].tris)
			ft_error("memory allocation failed");
	}
	level->ssp[x + y * SSP_MAX_X]
		.tris[level->ssp[x + y * SSP_MAX_X].tri_amount++] = tri;
}

void	find_ssp_index(t_tri tri, t_level *level)
{
	t_camera	cam;
	float		y_bounds[3];
	float		x_bounds[3];
	int			x;
	int			y;

	cam = level->cam;
	x_bounds[0] = 0;
	x_bounds[1] = RES_X - 1;
	x_bounds[2] = SSP_MAX_X - 1;
	y_bounds[0] = 0;
	y_bounds[1] = RES_Y - 1;
	y_bounds[2] = SSP_MAX_Y - 1;
	find_partition(horizontal, tri, cam, x_bounds);
	find_partition(vertical, tri, cam, y_bounds);
	x = get_ssp_coordinate(x_bounds[0], 1);
	while (x <= get_ssp_coordinate(x_bounds[1], 1))
	{
		y = get_ssp_coordinate(y_bounds[0], 0) - 1;
		while (++y <= get_ssp_coordinate(y_bounds[1], 0))
			ssp_set_tri(tri, level, x, y);
		x++;
	}
}

void	ssp_set_all(t_level *level, t_tri tri)
{
	int	x;
	int	y;

	y = 0;
	while (y < SSP_MAX_Y)
	{
		x = 0;
		while (x < SSP_MAX_X)
		{
			ssp_set_tri(tri, level, x, y);
			x++;
		}
		y++;
	}
}
