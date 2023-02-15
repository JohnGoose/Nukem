/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 23:23:49 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	deserialize_door(t_door *door, t_buffer *buf)
{
	int	i;

	deserialize_int(&door->indice_amount, buf);
	malloc_door(door);
	i = 0;
	while (i < door->indice_amount)
	{
		deserialize_int(&door->indices[i], buf);
		deserialize_int(&door->isquad[i], buf);
		door->pos1[i] = (t_vec3 *)malloc(sizeof(t_vec3) * 4);
		door->pos2[i] = (t_vec3 *)malloc(sizeof(t_vec3) * 4);
		if (!door->pos1[i] || !door->pos2[i])
			ft_error("memory allocation failed (deserialize_door)");
		deserialize_door_pos(door, buf, i);
		i++;
	}
	deserialize_int(&door->is_activation_pos_active, buf);
	deserialize_vec3(&door->activation_pos, buf);
	deserialize_float(&door->transition_time, buf);
}

void	free_doors(t_level *level)
{
	int	i;
	int	k;

	i = 0;
	while (i < level->doors.door_amount)
	{
		free(level->doors.door[i].indices);
		free(level->doors.door[i].isquad);
		k = 0;
		while (k < level->doors.door[i].indice_amount)
		{
			free(level->doors.door[i].pos1[k]);
			free(level->doors.door[i].pos2[k]);
			k++;
		}
		free(level->doors.door[i].pos1);
		free(level->doors.door[i].pos2);
		i++;
	}
	free(level->doors.door);
}

void	deserialize_doors(t_level *level, t_buffer *buf)
{
	int	i;

	i = 0;
	free_doors(level);
	deserialize_int(&level->doors.door_amount, buf);
	level->doors.door = (t_door *)malloc(sizeof(t_door)
			* level->doors.door_amount);
	if (!level->doors.door)
		ft_error("memory allocation failed (deserialize doors)\n");
	i = -1;
	while (++i < level->doors.door_amount)
		deserialize_door(&level->doors.door[i], buf);
}

void	deserialize_baked(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_int(&i, buf);
	level->bake_status = (t_bake)i;
	if (level->bake_status == BAKE_NOT_BAKED)
		return ;
	level->baked = ft_realloc(level->baked,
			sizeof(t_color) * level->baked_size.x * level->baked_size.y,
			sizeof(t_color) * level->texture.height * level->texture.width);
	i = 0;
	while (i < level->texture.height * level->texture.width)
	{
		deserialize_float(&level->baked[i].r, buf);
		deserialize_float(&level->baked[i].g, buf);
		deserialize_float(&level->baked[i].b, buf);
		i++;
	}
}

void	deserialize_level_images(t_level *level, t_buffer *buf)
{
	int	i;

	free(level->texture.image);
	deserialize_bmp(&level->texture, buf);
	free(level->normal_map.image);
	deserialize_bmp(&level->normal_map, buf);
	free(level->sky.img.image);
	deserialize_bmp(&level->sky.img, buf);
	free(level->spray.image);
	deserialize_bmp(&level->spray, buf);
	free(level->spray_overlay);
	level->spray_overlay = (unsigned *)malloc(
			sizeof(unsigned) * level->texture.width * level->texture.height);
	if (!level->spray_overlay)
		ft_error("memory allocation failed (deserialize level, spray_overlay)");
	i = 0;
	while (i < level->texture.height * level->texture.width)
	{
		deserialize_int((int *)&level->spray_overlay[i], buf);
		i++;
	}
	deserialize_baked(level, buf);
}
