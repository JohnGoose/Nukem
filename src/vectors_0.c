/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	lerp(float a, float b, float f)
{
	return (a + f * (b - a));
}

t_vec3	vec_interpolate(t_vec3 a, t_vec3 b, float f)
{
	t_vec3	res;

	res.x = lerp(a.x, b.x, f);
	res.y = lerp(a.y, b.y, f);
	res.z = lerp(a.z, b.z, f);
	return (res);
}

void	vec2_normalize(t_vec2 *vec)
{
	float	w;

	w = sqrtf(vec->x * vec->x + vec->y * vec->y);
	vec->x /= w;
	vec->y /= w;
}

void	vec_normalize(t_vec3 *vec)
{
	float	w;

	w = sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	vec->x /= w;
	vec->y /= w;
	vec->z /= w;
}

float	vec_length(t_vec3 vec)
{
	return (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}
