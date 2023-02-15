/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	deserialize_enemy_settings(t_enemy_settings *enemy, t_buffer *buf)
{
	deserialize_float(&enemy->dist_limit, buf);
	deserialize_float(&enemy->move_speed, buf);
	deserialize_float(&enemy->initial_health, buf);
	deserialize_float(&enemy->melee_range, buf);
	deserialize_float(&enemy->melee_damage, buf);
	deserialize_float(&enemy->attack_frequency, buf);
	deserialize_float(&enemy->move_duration, buf);
	deserialize_float(&enemy->shoot_duration, buf);
}

void	deserialize_projectile(t_projectile *projectile, t_buffer *buf)
{
	deserialize_float(&projectile->speed, buf);
	deserialize_float(&projectile->dist, buf);
	deserialize_float(&projectile->damage, buf);
	deserialize_float(&projectile->scale, buf);
}

void	deserialize_player_pos(t_player_pos *pos, t_buffer *buf)
{
	deserialize_vec3(&pos->pos, buf);
	deserialize_float(&pos->look_side, buf);
	deserialize_float(&pos->look_up, buf);
}

void	deserialize_perlin_settings(t_perlin_settings *p, t_buffer *buf)
{
	deserialize_float(&p->move_speed, buf);
	deserialize_float(&p->speed_diff, buf);
	deserialize_float(&p->scale, buf);
	deserialize_float(&p->min, buf);
	deserialize_float(&p->max, buf);
	deserialize_int(&p->resolution, buf);
	deserialize_float(&p->depth, buf);
	deserialize_float(&p->noise_opacity, buf);
	deserialize_float(&p->distance, buf);
	deserialize_float(&p->swirl, buf);
	deserialize_float(&p->swirl_interval, buf);
	deserialize_vec2(&p->dir, buf);
	deserialize_color(&p->color_1, buf);
	deserialize_color(&p->color_2, buf);
}

void	deserialize_tri(t_tri *tri, t_buffer *buf)
{
	int	i;

	i = -1;
	while (++i < 4)
		deserialize_vert(&tri->verts[i], buf);
	deserialize_vec3(&tri->v0v1, buf);
	deserialize_vec3(&tri->v0v2, buf);
	deserialize_vec3(&tri->normal, buf);
	deserialize_int(&tri->isquad, buf);
	deserialize_int(&tri->isgrid, buf);
	deserialize_int(&tri->isbreakable, buf);
	deserialize_int(&tri->opacity_precise, buf);
	deserialize_float(&tri->opacity, buf);
	deserialize_float(&tri->reflectivity, buf);
	deserialize_float(&tri->refractivity, buf);
	deserialize_int((int *)&tri->shader, buf);
	if (tri->shader == SHADER_PERLIN)
	{
		tri->perlin = (t_perlin_settings *)malloc(sizeof(t_perlin_settings));
		if (!tri->perlin)
			ft_error("failed to allocate memory for file");
		deserialize_perlin_settings(tri->perlin, buf);
	}
}
