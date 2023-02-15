/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	vec2_length(t_vec2 vec)
{
	return (sqrtf(vec.x * vec.x + vec.y * vec.y));
}

float	vec_dot(t_vec3 ve1, t_vec3 ve2)
{
	return (ve1.x * ve2.x + ve1.y * ve2.y + ve1.z * ve2.z);
}

int	vec_cmp(t_vec3 ve1, t_vec3 ve2)
{
	if (ve1.x != ve2.x)
		return (0);
	if (ve1.y != ve2.y)
		return (0);
	if (ve1.z != ve2.z)
		return (0);
	return (1);
}

void	vec2_avg(t_vec2 *res, t_vec2 ve1, t_vec2 ve2)
{
	res->x = (ve1.x + ve2.x) / 2;
	res->y = (ve1.y + ve2.y) / 2;
}

void	vec_avg(t_vec3 *res, t_vec3 ve1, t_vec3 ve2)
{
	res->x = (ve1.x + ve2.x) / 2;
	res->y = (ve1.y + ve2.y) / 2;
	res->z = (ve1.z + ve2.z) / 2;
}
