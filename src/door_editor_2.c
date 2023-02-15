/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_editor_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 20:59:31 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_door_pos(t_level *level, int is_pos2)
{
	t_door	*door;
	int		i;
	int		k;

	door = &level->doors.door[level->doors.selected_index - 1];
	i = 0;
	while (i < door->indice_amount)
	{
		k = 0;
		while (k < 4)
		{
			if (is_pos2)
				door->pos2[i][k]
					= level->all.tris[door->indices[i]].verts[k].pos;
			else
				door->pos1[i][k]
					= level->all.tris[door->indices[i]].verts[k].pos;
			k++;
		}
		i++;
	}
}

void	set_door_pos_1(t_level *level)
{
	set_door_pos(level, 0);
}

void	set_door_pos_2(t_level *level)
{
	set_door_pos(level, 1);
}

void	enable_door_editor(t_level *level)
{
	level->ui.state.ui_location = UI_LOCATION_DOOR_EDITOR;
	level->ui.wireframe = TRUE;
	level->ui.wireframe_on_top = TRUE;
	level->ui.wireframe_culling_visual = FALSE;
	level->ui.vertex_select_mode = FALSE;
}

void	reset_doors(t_level *level)
{
	t_door	*door;
	int		i;

	i = 0;
	while (i < level->doors.door_amount)
	{
		door = &level->doors.door[i];
		door->transition_start_time = UINT32_MAX;
		door->transition_direction = 1;
		i++;
	}
	door_animate(level);
}
