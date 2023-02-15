/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic_editor_0.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

float	obj_find_lowest_point(t_obj *obj)
{
	float	min;
	int		i;
	int		k;

	min = 0;
	i = 0;
	while (i < obj->tri_amount)
	{
		k = 0;
		while (k < 3 + obj->tris[i].isquad)
		{
			if (obj->tris[i].verts[k].pos.y > min)
				min = obj->tris[i].verts[k].pos.y;
			k++;
		}
		i++;
	}
	return (min);
}

int	obj_pos_set_to_floor(t_vec3 *vec, t_obj *obj, t_level *level)
{
	t_ray	ray;
	float	ground_dist;

	ray.dir = (t_vec3){0, 1, 0};
	ray.pos = *vec;
	ground_dist = cast_all(ray, level, NULL);
	if (ground_dist == FLT_MAX)
		ground_dist = 0;
	else
		ground_dist -= obj_find_lowest_point(obj);
	vec->y += ground_dist;
	return (ground_dist != FALSE);
}

void	add_health_box(t_level *level)
{
	int	amount;

	amount = level->game_logic.health_box_amount;
	level->game_logic.health_box
		= (t_item_pickup *)ft_realloc(level->game_logic.health_box,
			sizeof(t_item_pickup) * amount,
			sizeof(t_item_pickup) * (amount + 1));
	if (!level->game_logic.health_box)
		ft_error("memory allocation failed");
	vec_add(&level->game_logic.health_box[amount].pos,
		level->cam.pos, level->cam.front);
	level->game_logic.health_box[amount].visible = 1;
	level->game_logic.health_box[amount].start_time = 0;
	obj_pos_set_to_floor(&level->game_logic.health_box[amount].pos,
		&level->game_models.health_pickup_box, level);
	level->game_logic.health_box_amount++;
}

void	add_ammo_box(t_level *level)
{
	int	amount;

	amount = level->game_logic.ammo_box_amount;
	level->game_logic.ammo_box
		= (t_item_pickup *)ft_realloc(level->game_logic.ammo_box,
			sizeof(t_item_pickup) * amount,
			sizeof(t_item_pickup) * (amount + 1));
	if (!level->game_logic.ammo_box)
		ft_error("memory allocation failed");
	vec_add(&level->game_logic.ammo_box[amount].pos,
		level->cam.pos, level->cam.front);
	level->game_logic.ammo_box[amount].visible = 1;
	level->game_logic.ammo_box[amount].start_time = 0;
	obj_pos_set_to_floor(&level->game_logic.ammo_box[amount].pos,
		&level->game_models.ammo_pickup_box, level);
	level->game_logic.ammo_box_amount++;
}

void	delete_health_box(t_level *level)
{
	int	amount;
	int	i;

	amount = level->game_logic.health_box_amount;
	i = level->ui.state.logic_selected_index;
	while (i < amount - 1)
	{
		level->game_logic.health_box[i] = level->game_logic.health_box[i + 1];
		i++;
	}
	level->game_logic.health_box
		= (t_item_pickup *)ft_realloc(level->game_logic.health_box,
			sizeof(t_item_pickup) * amount,
			sizeof(t_item_pickup) * (amount - 1));
	if (!level->game_logic.health_box)
		ft_error("memory allocation failed");
	level->game_logic.health_box_amount--;
	level->ui.state.logic_selected = GAME_LOGIC_SELECTED_NONE;
}
