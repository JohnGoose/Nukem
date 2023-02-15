/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_geometry_0.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/06 00:11:14 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	visible_request_merge(t_level *level, int amount)
{
	int	prev_amount;

	if (!level->visible_max)
		level->visible_max = 100;
	if (amount + level->visible.tri_amount
		>= level->visible_max)
	{
		prev_amount = level->visible_max;
		while (amount + level->visible.tri_amount
			>= level->visible_max)
			level->visible_max *= 1.5;
		level->visible.tris = (t_tri *)ft_realloc(level->visible.tris,
				sizeof(t_tri) * prev_amount,
				sizeof(t_tri) * level->visible_max);
		if (!level->visible.tris)
			ft_error("memory allocation failed");
	}
}

void	merge_mesh(t_level *level, t_obj *obj, t_vec3 pos, t_vec2 rotation)
{
	int	i;
	int	k;
	int	z;

	visible_request_merge(level, obj->tri_amount);
	k = -1;
	i = level->visible.tri_amount;
	while (++k < obj->tri_amount)
	{
		level->visible.tris[i] = obj->tris[k];
		level->visible.tris[i].index = 0;
		z = -1;
		while (++z < 3 + obj->tris[k].isquad)
		{
			rotate_vertex(rotation.x, &level->visible.tris[i].verts[z].pos, 1);
			rotate_vertex(rotation.y, &level->visible.tris[i].verts[z].pos, 0);
			vec_add(&level->visible.tris[i].verts[z].pos,
				level->visible.tris[i].verts[z].pos, pos);
		}
		tri_optimize(&level->visible.tris[i], true);
		level->visible.tris[i].texture = &obj->texture;
		i++;
	}
	level->visible.tri_amount = i;
}

void	merge_enemies_runtime(t_level *level)
{
	int	i;

	i = -1;
	while (++i < level->game_logic.enemy_amount)
	{
		if (level->game_logic.enemies[i].dead_start_time)
			play_animation(&level->game_models.enemy,
				&level->game_models.enemy_die,
				level->game_logic.enemies[i].dead_start_time);
		else
		{
			if (level->game_logic.enemies[i].shoot_start_time)
				obj_copy(&level->game_models.enemy,
					&level->game_models.enemy_shoot);
			else
				play_animation(&level->game_models.enemy,
					&level->game_models.enemy_run, 0);
		}
		merge_mesh(level, &level->game_models.enemy,
			level->game_logic.enemies[i].pos,
			(t_vec2){0, level->game_logic.enemies[i].dir_rad});
	}
}

void	merge_enemies_editor(t_level *level, float rot)
{
	int	i;

	if (level->game_logic.enemy_animation_view_index == 0)
		play_animation(&level->game_models.enemy,
			&level->game_models.enemy_run, 0);
	else if (level->game_logic.enemy_animation_view_index == 1)
		play_animation(&level->game_models.enemy,
			&level->game_models.enemy_die, 0);
	else
		obj_copy(&level->game_models.enemy, &level->game_models.enemy_shoot);
	i = -1;
	while (++i < level->game_logic.enemy_amount)
		merge_mesh(level, &level->game_models.enemy,
			level->game_logic.enemies[i].spawn_pos,
			(t_vec2){0, rot + (M_PI / 3 * i)});
}

void	merge_pickups(t_level *level, float rot)
{
	int	i;

	i = -1;
	while (++i < level->game_logic.ammo_box_amount)
		if (level->game_logic.ammo_box[i].visible)
			merge_mesh(level, &level->game_models.ammo_pickup_box,
				level->game_logic.ammo_box[i].pos,
				(t_vec2){0, rot + (M_PI / 3 * i)});
	i = -1;
	while (++i < level->game_logic.health_box_amount)
		if (level->game_logic.health_box[i].visible)
			merge_mesh(level, &level->game_models.health_pickup_box,
				level->game_logic.health_box[i].pos,
				(t_vec2){0, rot + (M_PI / 3 * i)});
}
