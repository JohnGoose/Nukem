/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_editor_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	door_activation_move(t_level *level, t_vec3 move_amount)
{
	int	index;

	index = level->doors.selected_index - 1;
	vec_add(&level->doors.door[index].activation_pos,
		level->doors.door[index].activation_pos, move_amount);
	level->ui.state.gizmo_pos = level->doors.door[index].activation_pos;
}

void	delete_all_doors(t_level *level)
{
	t_door	*door;
	int		i;
	int		k;

	k = -1;
	while (++k < level->doors.door_amount)
	{
		door = &level->doors.door[k];
		i = 0;
		while (i < door->indice_amount)
		{
			free(door->pos1[i]);
			free(door->pos2[i]);
			i++;
		}
		free(door->pos1);
		free(door->pos2);
		free(door->indices);
		free(door->isquad);
	}
	level->doors.door_amount = 0;
	free(level->doors.door);
	level->doors.door = NULL;
	deselect_all_faces(level);
	level->doors.selected_index = 0;
}

void	delete_door(t_level *level)
{
	t_door	*door;
	int		i;

	door = &level->doors.door[level->doors.selected_index - 1];
	i = 0;
	while (i < door->indice_amount)
	{
		free(door->pos1[i]);
		free(door->pos2[i]);
		i++;
	}
	free(door->pos1);
	free(door->pos2);
	free(door->indices);
	free(door->isquad);
	i = level->doors.selected_index - 2;
	while (++i < level->doors.door_amount - 1)
		level->doors.door[i] = level->doors.door[i + 1];
	level->doors.door_amount--;
	level->doors.door = (t_door *)ft_realloc(level->doors.door,
			sizeof(t_door) * level->doors.door_amount - 1,
			sizeof(t_door) * level->doors.door_amount);
	if (!level->doors.door)
		ft_error("memory allocation failed\n");
	deselect_door(level);
}

t_door	*alloc_new_door(t_level *level)
{
	t_door	*door;
	int		selected;
	int		i;

	level->doors.door_amount++;
	level->doors.door = (t_door *)ft_realloc(level->doors.door,
			sizeof(t_door) * level->doors.door_amount,
			sizeof(t_door) * level->doors.door_amount + 1);
	if (!level->doors.door)
		ft_error("memory allocation failed\n");
	door = &level->doors.door[level->doors.door_amount - 1];
	ft_bzero(door, sizeof(t_door));
	selected = 0;
	i = -1;
	while (++i < level->all.tri_amount)
		if (level->all.tris[i].selected)
			selected++;
	door->indice_amount = selected;
	door->isquad = (int *)malloc(sizeof(int) * selected);
	door->indices = (int *)malloc(sizeof(int) * selected);
	door->pos1 = (t_vec3 **)malloc(sizeof(t_vec3 *) * selected);
	door->pos2 = (t_vec3 **)malloc(sizeof(t_vec3 *) * selected);
	if (!door->pos2 || !door->pos1 || !door->indices || !door->isquad)
		ft_error("memory allocation failed\n");
	return (door);
}

void	add_new_door(t_level *level)
{
	t_door	*door;
	t_ivec3	i;

	door = alloc_new_door(level);
	i.z = 0;
	i.x = -1;
	while (++i.x < level->all.tri_amount)
	{
		if (level->all.tris[i.x].selected)
		{
			door->isquad[i.z] = level->all.tris[i.x].isquad;
			door->indices[i.z] = i.x;
			door->pos1[i.z] = (t_vec3 *)malloc(sizeof(t_vec3) * 4);
			door->pos2[i.z] = (t_vec3 *)malloc(sizeof(t_vec3) * 4);
			if (!door->pos2[i.z] || !door->pos1[i.z])
				ft_error("memory allocation failed\n");
			i.y = -1;
			while (++i.y < 4)
			{
				door->pos1[i.z][i.y] = level->all.tris[i.x].verts[i.y].pos;
				door->pos2[i.z][i.y] = level->all.tris[i.x].verts[i.y].pos;
			}
			i.z++;
		}
	}
}
