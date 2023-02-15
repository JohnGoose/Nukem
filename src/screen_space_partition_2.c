/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_space_partition_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	screen_space_partition(t_level *level)
{
	int	i;

	i = -1;
	while (++i < SSP_MAX_X * SSP_MAX_Y)
		level->ssp[i].tri_amount = 0;
	i = 0;
	while (i < level->visible.tri_amount)
	{
		if (level->visible.tris[i].isgrid)
			ssp_set_all(level, level->visible.tris[i]);
		else
			find_ssp_index(level->visible.tris[i], level);
		i++;
	}
}

void	init_screen_space_partition(t_level *level)
{
	int	i;

	level->ssp = (t_obj *)malloc(sizeof(t_obj) * SSP_MAX_X * SSP_MAX_Y);
	if (!level->ssp)
		ft_error("memory allocation failed");
	i = 0;
	while (i < SSP_MAX_X * SSP_MAX_Y)
	{
		level->ssp[i].tris = (t_tri *)malloc(sizeof(t_tri)
				* SSP_INITIAL_SIZE);
		if (!level->ssp[i].tris)
			ft_error("memory allocation failed");
		level->ssp_max[i] = SSP_INITIAL_SIZE;
		i++;
	}
}
