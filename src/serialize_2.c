/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	serialize_obj(t_obj *obj, t_buffer *buf)
{
	int	i;

	i = 0;
	serialize_int(obj->tri_amount, buf);
	while (i < obj->tri_amount)
	{
		serialize_tri(&obj->tris[i], buf);
		i++;
	}
}

void	serialize_bmp(t_bmp *bmp, t_buffer *buf)
{
	int	x;
	int	y;

	serialize_int(bmp->width, buf);
	serialize_int(bmp->height, buf);
	y = 0;
	while (y < bmp->height)
	{
		x = 0;
		while (x < bmp->width)
		{
			serialize_int(bmp->image[bmp->width * y + x], buf);
			x++;
		}
		y++;
	}
}

void	serialize_string(char *str, t_buffer *buf)
{
	int	i;

	i = 0;
	while (str[i])
	{
		serialize_int(str[i], buf);
		i++;
	}
}

void	serialize_door(t_door *door, t_buffer *buf)
{
	int	i;
	int	k;

	i = 0;
	serialize_int(door->indice_amount, buf);
	while (i < door->indice_amount)
	{
		serialize_int(door->indices[i], buf);
		serialize_int(door->isquad[i], buf);
		k = 0;
		while (k < 4)
		{
			serialize_vec3(door->pos1[i][k], buf);
			serialize_vec3(door->pos2[i][k], buf);
			k++;
		}
		i++;
	}
	serialize_int(door->is_activation_pos_active, buf);
	serialize_vec3(door->activation_pos, buf);
	serialize_float(door->transition_time, buf);
}

void	serialize_doors(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_int(level->doors.door_amount, buf);
	i = 0;
	while (i < level->doors.door_amount)
	{
		serialize_door(&level->doors.door[i], buf);
		i++;
	}
}
