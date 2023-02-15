/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bloom_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_color	*get_buff(t_color *set)
{
	static t_color	*buff = NULL;

	if (set)
		buff = set;
	return (buff);
}

unsigned int	color_to_int(t_color color)
{
	unsigned int	res;
	unsigned char	*rgb;

	rgb = (unsigned char *)&res;
	color.r = clamp(color.r, 0, 1);
	color.g = clamp(color.g, 0, 1);
	color.b = clamp(color.b, 0, 1);
	rgb[3] = color.r * 0xff;
	rgb[2] = color.g * 0xff;
	rgb[1] = color.b * 0xff;
	rgb[0] = 0xff;
	return (res);
}

t_color	int_to_color(unsigned int color)
{
	unsigned char	*rgb;
	t_color			res;

	rgb = (unsigned char *)&color;
	res.r = rgb[3] / (float)0xff;
	res.g = rgb[2] / (float)0xff;
	res.b = rgb[1] / (float)0xff;
	return (res);
}

void	color_cpy(t_color *c1, t_color *c2)
{
	c1->r = c2->r;
	c1->g = c2->g;
	c1->b = c2->b;
}

void	color_add(t_color *c1, t_color *c2)
{
	c1->r += c2->r;
	c1->g += c2->g;
	c1->b += c2->b;
}
