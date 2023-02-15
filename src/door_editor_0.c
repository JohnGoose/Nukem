/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_editor_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	select_door(int index, t_level *level)
{
	int	i;

	deselect_all_faces(level);
	level->doors.selected_index = index + 1;
	i = -1;
	while (++i < level->doors.door[index].indice_amount)
		level->all.tris[level->doors.door[index].indices[i]]
			.selected = TRUE;
}

void	deselect_door(t_level *level)
{
	if (level->doors.selected_index)
		deselect_all_faces(level);
	level->doors.selected_index = 0;
}

void	find_selected_door_index(t_level *level)
{
	int	i;
	int	k;
	int	m;

	i = -1;
	while (++i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
		{
			k = -1;
			while (++k < level->doors.door_amount)
			{
				m = -1;
				while (++m < level->doors.door[k].indice_amount)
				{
					if (i == level->doors.door[k].indices[m])
						return (select_door(k, level));
				}
			}
		}
	}
	deselect_door(level);
}

void	door_find_avg(t_level *level)
{
	t_ivec3	i;
	t_door	*door;
	int		avg_amount;

	i.x = -1;
	while (++i.x < level->doors.door_amount)
	{
		door = &level->doors.door[i.x];
		vec_mult(&door->avg, 0);
		avg_amount = 0;
		i.y = -1;
		while (++i.y < door->indice_amount)
		{
			i.z = -1;
			while (++i.z < 3 + door->isquad[i.y])
			{
				vec_add(&door->avg, door->avg,
					level->all.tris[door->indices[i.y]].verts[i.z].pos);
				avg_amount++;
			}
		}
		if (avg_amount)
			vec_div(&door->avg, avg_amount);
	}
}

void	door_put_text(t_level *level)
{
	t_door	*door;
	char	buf[100];
	int		i;

	door_find_avg(level);
	i = 0;
	while (i < level->doors.door_amount)
	{
		door = &level->doors.door[i];
		ft_sprintf(buf, "door %d", i + 1);
		render_text_3d(buf, door->avg, DOOR_LOCATION_INFO_COLOR, level);
		if (door->is_activation_pos_active)
		{
			ft_sprintf(buf, "door %d activation", i + 1);
			render_text_3d(buf, door->activation_pos,
				DOOR_ACTIVATION_LOCATION_INFO_COLOR, level);
		}
		i++;
	}
}
