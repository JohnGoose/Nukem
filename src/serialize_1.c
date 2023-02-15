/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	serialize_enemy_settings(t_enemy_settings *enemy, t_buffer *buf)
{
	serialize_float(enemy->dist_limit, buf);
	serialize_float(enemy->move_speed, buf);
	serialize_float(enemy->initial_health, buf);
	serialize_float(enemy->melee_range, buf);
	serialize_float(enemy->melee_damage, buf);
	serialize_float(enemy->attack_frequency, buf);
	serialize_float(enemy->move_duration, buf);
	serialize_float(enemy->shoot_duration, buf);
}

void	serialize_projectile(t_projectile *projectile, t_buffer *buf)
{
	serialize_float(projectile->speed, buf);
	serialize_float(projectile->dist, buf);
	serialize_float(projectile->damage, buf);
	serialize_float(projectile->scale, buf);
}

void	serialize_player_pos(t_player_pos *pos, t_buffer *buf)
{
	serialize_vec3(pos->pos, buf);
	serialize_float(pos->look_side, buf);
	serialize_float(pos->look_up, buf);
}

void	serialize_perlin_settings(t_perlin_settings *p, t_buffer *buf)
{
	serialize_float(p->move_speed, buf);
	serialize_float(p->speed_diff, buf);
	serialize_float(p->scale, buf);
	serialize_float(p->min, buf);
	serialize_float(p->max, buf);
	serialize_int(p->resolution, buf);
	serialize_float(p->depth, buf);
	serialize_float(p->noise_opacity, buf);
	serialize_float(p->distance, buf);
	serialize_float(p->swirl, buf);
	serialize_float(p->swirl_interval, buf);
	serialize_vec2(p->dir, buf);
	serialize_color(p->color_1, buf);
	serialize_color(p->color_2, buf);
}

void	serialize_tri(t_tri *tri, t_buffer *buf)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		serialize_vert(&tri->verts[i], buf);
		i++;
	}
	serialize_vec3(tri->v0v1, buf);
	serialize_vec3(tri->v0v2, buf);
	serialize_vec3(tri->normal, buf);
	serialize_int(tri->isquad, buf);
	serialize_int(tri->isgrid, buf);
	serialize_int(tri->isbreakable, buf);
	serialize_int(tri->opacity_precise, buf);
	serialize_float(tri->opacity, buf);
	serialize_float(tri->reflectivity, buf);
	serialize_float(tri->refractivity, buf);
	serialize_int((int)tri->shader, buf);
	if (tri->shader == SHADER_PERLIN)
		serialize_perlin_settings(tri->perlin, buf);
}
