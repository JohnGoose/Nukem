/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	vec2_add(t_vec2 *res, t_vec2 ve1, t_vec2 ve2)
{
	res->x = ve1.x + ve2.x;
	res->y = ve1.y + ve2.y;
}

void	vec2_mult(t_vec2 *res, float mult)
{
	res->x *= mult;
	res->y *= mult;
}

void	vec_add(t_vec3 *res, t_vec3 ve1, t_vec3 ve2)
{
	res->x = ve1.x + ve2.x;
	res->y = ve1.y + ve2.y;
	res->z = ve1.z + ve2.z;
}

void	vec2_sub(t_vec2 *res, t_vec2 ve1, t_vec2 ve2)
{
	res->x = ve1.x - ve2.x;
	res->y = ve1.y - ve2.y;
}

void	vec_sub(t_vec3 *res, t_vec3 ve1, t_vec3 ve2)
{
	res->x = ve1.x - ve2.x;
	res->y = ve1.y - ve2.y;
	res->z = ve1.z - ve2.z;
}
