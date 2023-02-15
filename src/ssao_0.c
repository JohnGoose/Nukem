/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	pixel_dot_product(t_ivec2 i, t_ivec2 mid, t_window *win)
{
	t_vec3	v1;
	t_vec3	v2;

	v2 = win->pixel_pos_buffer[i.x + i.y * RES_X];
	v1 = win->pixel_pos_buffer[mid.x + mid.y * RES_X];
	vec_sub(&v2, v2, v1);
	vec_normalize(&v2);
	vec_normalize(&v1);
	return (-vec_dot(v2, v1));
}

void	ssao_kernel_iter(t_ssao *ssao, t_window *win, t_ivec2 i, float br)
{
	float	dist;
	float	diff;
	float	val;

	dist = win->depth_buffer[i.x + i.y * RES_X];
	diff = win->depth_buffer[ssao->kernel_center.x
		+ ssao->kernel_center.y * RES_X] - dist;
	val = pixel_dot_product(i, ssao->kernel_center, win);
	if (dist != FLT_MAX && !isnan(val))
	{
		val /= br;
		if (diff > 1)
		{
			val /= diff;
			ssao->count += diff - 1;
		}
		ssao->total += radial_gradient(i, ssao->lower_bound, ssao->radius * 2)
			* val;
	}
	else if (dist == FLT_MAX)
		ssao->total -= radial_gradient(i, ssao->lower_bound, ssao->radius * 2);
	ssao->count++;
}

void	ssao_bounds(t_ssao *ssao)
{
	ssao->lower_bound.x = ssao->kernel_center.x - ssao->radius;
	ssao->lower_bound.y = ssao->kernel_center.y - ssao->radius;
	ssao->upper_bound.x = ssao->kernel_center.x + ssao->radius;
	ssao->upper_bound.y = ssao->kernel_center.y + ssao->radius;
	if (ssao->lower_bound.y < 0)
		ssao->lower_bound.y = 0;
	if (ssao->lower_bound.x < 0)
		ssao->lower_bound.x = 0;
	if (ssao->upper_bound.y > RES_Y)
		ssao->upper_bound.y = RES_Y;
	if (ssao->upper_bound.x > RES_X)
		ssao->upper_bound.x = RES_X;
}

float	ssao_avg(t_ssao ssao, t_level *level)
{
	float	avg;

	avg = 0;
	if (ssao.count <= 0)
		ssao.total = 0;
	if (ssao.total)
		avg = ssao.total / (float)ssao.count;
	avg *= level->ui.ssao_intensity;
	if (avg > 1)
		return (1);
	else if (avg < 0)
		return (0);
	return (avg);
}

float	surrounding_diff(t_ssao ssao, t_level *level, t_window *win)
{
	t_ivec2	i;
	t_color	light;
	float	brightness;

	ssao_bounds(&ssao);
	ssao.count = 0;
	ssao.total = 0;
	i.x = ssao.lower_bound.x;
	while (i.x < ssao.upper_bound.x)
	{
		i.y = ssao.lower_bound.y;
		while (i.y < ssao.upper_bound.y)
		{
			light = win->brightness_buffer[i.x + i.y * RES_X];
			brightness = (light.r + light.g + light.b)
				* level->ui.ssao_light_bias;
			if (brightness > 1)
				ssao_kernel_iter(&ssao, win, i, brightness);
			else
				ssao_kernel_iter(&ssao, win, i, 1);
			i.y += level->ui.raycast_quality;
		}
		i.x += level->ui.raycast_quality;
	}
	return (ssao_avg(ssao, level));
}
