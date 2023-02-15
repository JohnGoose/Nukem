/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_perlin_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	noise_move(float time, t_vec3 pos, t_perlin_settings p)
{
	if (p.swirl)
		return (noise_swirl(time, pos, p));
	pos.x += p.dir.x * time + PERLIN_OFFSET;
	pos.z += p.dir.y * time + PERLIN_OFFSET;
	return (perlin_noise(pos.x, pos.z, p.scale, p.resolution));
}

float	perlin_opacity(t_vec3 *pos, float perlin, t_level *l,
						t_perlin_settings p)
{
	t_vec3	tmp;
	float	time;

	time = SDL_GetTicks() / 1000.0 * p.move_speed * p.speed_diff;
	vec_sub(&tmp, *pos, l->cam.pos);
	vec_normalize(&tmp);
	vec_mult(&tmp, perlin * p.depth);
	vec_add(pos, *pos, tmp);
	perlin = noise_move(time, *pos, p);
	perlin = stretch_value(perlin, p.min, p.max);
	return (perlin);
}

unsigned int	depth_grid_visualizer(t_vec3 pos,
											t_level *level, t_cast_result *res)
{
	if ((fmod((pos.x), 4) > 2) ^ (fmod((pos.z), 4) > 2))
		return (0xffffffff);
	if (level->all.tris[res->face_index].opacity)
		return (0);
	return (0x000000ff);
}

unsigned int	shader_perlin(t_vec3 pos, t_level *level, t_cast_result *res)
{
	t_perlin_settings	p;
	float				time;
	float				perlin;
	float				opacity;

	if (!level->all.tris[res->face_index].perlin)
		return (0x000000ff);
	res->normal = level->all.tris[res->face_index].normal;
	p = *level->all.tris[res->face_index].perlin;
	time = SDL_GetTicks() / 1000.0 * p.move_speed;
	perlin = noise_move(time, pos, p);
	perlin = stretch_value(perlin, p.min, p.max);
	opacity = noise_opacity(p, perlin, *res);
	if (p.visualizer == 1)
		return (crossfade(p.color_1.color >> 8,
				p.color_2.color >> 8, perlin * 0xff, opacity * 0xff));
	res->normal.x += perlin - 0.5;
	perlin = perlin_opacity(&pos, perlin, level, p);
	if (p.visualizer == 2)
		return (depth_grid_visualizer(pos, level, res));
	opacity = noise_opacity(p, perlin, *res);
	return (crossfade(p.color_1.color >> 8,
			p.color_2.color >> 8, perlin * 0xff, opacity * 0xff));
}
