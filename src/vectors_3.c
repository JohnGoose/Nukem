/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	vec_cross(t_vec3 *res, t_vec3 u, t_vec3 v)
{
	res->x = u.y * v.z - v.y * u.z;
	res->y = v.x * u.z - u.x * v.z;
	res->z = u.x * v.y - v.x * u.y;
}

void	vec_rot(t_vec3 *res, t_vec3 ve1, float ang)
{
	float	c;
	float	s;

	c = cosf(ang);
	s = sinf(ang);
	res->x = ve1.x * c + ve1.z * s;
	res->y = ve1.y;
	res->z = -ve1.x * s + ve1.z * c;
}

float	vec_angle(t_vec3 v1, t_vec3 v2)
{
	float	dot;
	float	len;

	dot = vec_dot(v1, v2);
	len = vec_length(v1) * vec_length(v2);
	return (acosf(dot / len));
}

void	vec_mult(t_vec3 *res, float mult)
{
	res->x = res->x * mult;
	res->y = res->y * mult;
	res->z = res->z * mult;
}

void	vec_div(t_vec3 *res, float div)
{
	res->x = res->x / div;
	res->y = res->y / div;
	res->z = res->z / div;
}
