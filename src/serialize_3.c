/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 19:47:26 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	serialize_lights(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_int(level->light_amount, buf);
	i = 0;
	while (i < level->light_amount)
	{
		serialize_vec3(level->lights[i].pos, buf);
		serialize_color(level->lights[i].color, buf);
		serialize_float(level->lights[i].radius, buf);
		serialize_float(level->lights[i].power, buf);
		i++;
	}
}

void	serialize_level_images(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_bmp(&level->texture, buf);
	serialize_bmp(&level->normal_map, buf);
	serialize_bmp(&level->sky.img, buf);
	serialize_bmp(&level->spray, buf);
	i = 0;
	while (i < level->texture.height * level->texture.width)
	{
		serialize_int(level->spray_overlay[i], buf);
		i++;
	}
	i = (int)level->bake_status;
	serialize_int(i, buf);
	if (level->bake_status == BAKE_NOT_BAKED)
		return ;
	i = 0;
	while (i < level->texture.height * level->texture.width)
	{
		serialize_float(level->baked[i].r, buf);
		serialize_float(level->baked[i].g, buf);
		serialize_float(level->baked[i].b, buf);
		i++;
	}
}

void	serialize_pickups(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_int(level->game_logic.health_box_amount, buf);
	i = -1;
	while (++i < level->game_logic.health_box_amount)
		serialize_vec3(level->game_logic.health_box[i].pos, buf);
	serialize_int(level->game_logic.ammo_box_amount, buf);
	i = -1;
	while (++i < level->game_logic.ammo_box_amount)
		serialize_vec3(level->game_logic.ammo_box[i].pos, buf);
}

void	serialize_enemies(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_enemy_settings(&level->game_logic.enemy_settings, buf);
	serialize_int(level->game_logic.enemy_amount, buf);
	i = -1;
	while (++i < level->game_logic.enemy_amount)
		serialize_vec3(level->game_logic.enemies[i].spawn_pos, buf);
}

void	serialize_menu_anim(t_level *level, t_buffer *buf)
{
	int	i;

	serialize_int(level->main_menu_anim.amount, buf);
	serialize_int(level->main_menu_anim.duration, buf);
	serialize_int(level->main_menu_anim.loop, buf);
	i = -1;
	while (++i < level->main_menu_anim.amount)
		serialize_player_pos(&level->main_menu_anim.pos[i], buf);
}
