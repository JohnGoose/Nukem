/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	deserialize_obj(t_obj *obj, t_buffer *buf)
{
	int	i;

	i = 0;
	deserialize_int(&obj->tri_amount, buf);
	obj->tris = (t_tri *)malloc(sizeof(t_tri) * obj->tri_amount);
	if (!obj->tris)
		ft_error("failed to allocate memory for file (deserialize_obj)");
	ft_bzero(obj->tris, sizeof(t_tri) * obj->tri_amount);
	while (i < obj->tri_amount)
	{
		deserialize_tri(&obj->tris[i], buf);
		obj->tris[i].index = i;
		i++;
	}
}

void	deserialize_bmp(t_bmp *bmp, t_buffer *buf)
{
	int	x;
	int	y;

	deserialize_int(&bmp->width, buf);
	deserialize_int(&bmp->height, buf);
	bmp->image = (int *)malloc(sizeof(int) * bmp->width * bmp->height);
	if (!bmp->image)
		ft_error("failed to allocate memory for file (deserialize_bmp)");
	y = 0;
	while (y < bmp->height)
	{
		x = 0;
		while (x < bmp->width)
		{
			deserialize_int(&bmp->image[bmp->width * y + x], buf);
			x++;
		}
		y++;
	}
}

char	*deserialize_string(int len, t_buffer *buf)
{
	char	*str;
	int		get;
	int		i;

	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		ft_error("memory allocation failed (deserialize_string)");
	i = 0;
	while (i < len)
	{
		deserialize_int(&get, buf);
		str[i] = (char)get;
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	malloc_door(t_door *door)
{
	door->indices = (int *)malloc(sizeof(int) * door->indice_amount);
	door->isquad = (int *)malloc(sizeof(int) * door->indice_amount);
	door->pos1 = (t_vec3 **)malloc(sizeof(t_vec3 *) * door->indice_amount);
	door->pos2 = (t_vec3 **)malloc(sizeof(t_vec3 *) * door->indice_amount);
	if (!door->pos1 || !door->pos2 || !door->isquad || !door->indices)
		ft_error("memory allocation failed (deserialize malloc_door)");
}

void	deserialize_door_pos(t_door *door, t_buffer *buf, int door_index)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		deserialize_vec3(&door->pos1[door_index][i], buf);
		deserialize_vec3(&door->pos2[door_index][i], buf);
		i++;
	}
}
