/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_overlap_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	fix_uv_overlap(t_level *level)
{
	int		i;
	int		j;
	t_uv	uv;

	i = 0;
	while (i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
		{
			j = 0;
			while (j < level->all.tri_amount)
			{
				if (i != j
					&& tri_uv_intersect(level->all.tris[i], level->all.tris[j]))
					move_uv(&level->all.tris[i], i, level, &uv);
				j++;
			}
		}
		i++;
	}
}
