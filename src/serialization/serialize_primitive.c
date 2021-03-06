/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_primitive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:35:03 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/15 15:46:00 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	reserve_space(t_buffer *buf, size_t bytes)
{
	if ((buf->next + bytes) > buf->size)
	{
		buf->data = realloc(buf->data, buf->size * 2);
		if (!buf->data)
			ft_error("memory allocation failed\n");
		buf->size *= 2;
	}
}

static float	hton_float(float value)
{
	int		temp;
	void	*cast;

	cast = &value;
	temp = htonl(*(unsigned int *)cast);
	cast = &temp;
	return (*(float *)cast);
}

void	serialize_float(float x, t_buffer *buf)
{
	x = hton_float(x);
	reserve_space(buf, sizeof(float));
	ft_memcpy(((char *)buf->data) + buf->next, &x, sizeof(float));
	buf->next += sizeof(float);
}

void	serialize_int(int x, t_buffer *buf)
{
	x = htonl(x);
	reserve_space(buf, sizeof(int));
	ft_memcpy(((char *)buf->data) + buf->next, &x, sizeof(int));
	buf->next += sizeof(int);
}
