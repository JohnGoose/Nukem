/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	toggle_selection_all(t_level *level)
{
	int	selected;
	int	i;
	int	j;

	i = 0;
	selected = FALSE;
	while (i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
			selected = TRUE;
		i++;
	}
	i = 0;
	while (i < level->all.tri_amount)
	{
		level->all.tris[i].selected = !selected;
		j = 0;
		while (j < 3 + level->all.tris[i].isquad)
		{
			level->all.tris[i].verts[j].selected = !selected;
			j++;
		}
		i++;
	}
}
