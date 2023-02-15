/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_perlin_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	noise2(int x, int y)
{
	static int	*hash = NULL;
	int			tmp;

	if (!hash)
	{
		hash = (int *)malloc(sizeof(int) * 256);
		if (!hash)
		{
			nonfatal_error("memory allocation failed for perlin noise");
			return (0);
		}
		tmp = 0;
		while (tmp < 256)
		{
			hash[tmp] = rand() % 256;
			tmp++;
		}
	}
	tmp = (hash[y % 256] + x);
	return (hash[tmp % 256]);
}

float	lin_inter(float x, float y, float s)
{
	return (x + s * (y - x));
}

float	smooth_inter(float x, float y, float s)
{
	return (lin_inter(x, y, s * s * (3 - 2 * s)));
}

float	noise2d(float x, float y)
{
	t_ivec2	in;
	t_vec2	frac;
	float	low;
	float	high;

	in.x = (int)x;
	in.y = (int)y;
	frac.x = x - in.x;
	frac.y = y - in.y;
	low = smooth_inter(noise2(in.x, in.y),
			noise2(in.x + 1, in.y), frac.x);
	high = smooth_inter(noise2(in.x, in.y + 1),
			noise2(in.x + 1, in.y + 1), frac.x);
	return (smooth_inter(low, high, frac.y));
}

float	perlin_noise(float x, float y, float freq, int depth)
{
	float	amp;
	float	fin;
	float	div;
	int		i;

	i = 0;
	fin = 0;
	div = 0;
	amp = 1;
	x = fabs(x);
	y = fabs(y);
	x *= freq;
	y *= freq;
	while (i < depth)
	{
		div += 256 * amp;
		fin += noise2d(x, y) * amp;
		amp /= 2;
		x *= 2;
		y *= 2;
		i++;
	}
	return (fin / div);
}
