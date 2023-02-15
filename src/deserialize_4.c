/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 01:08:45 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	deserialize_health_pickups(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_int(&level->game_logic.health_box_amount, buf);
	if (level->game_logic.health_box)
		free(level->game_logic.health_box);
	level->game_logic.health_box = (t_item_pickup *)malloc(
			sizeof(t_item_pickup) * level->game_logic.health_box_amount);
	if (!level->game_logic.health_box)
		ft_error("memory allocation failed (deserialize level, health)");
	i = -1;
	while (++i < level->game_logic.health_box_amount)
	{
		deserialize_vec3(&level->game_logic.health_box[i].pos, buf);
		level->game_logic.health_box[i].start_time = 0;
		level->game_logic.health_box[i].visible = 1;
	}
}

void	deserialize_ammo_pickups(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_int(&level->game_logic.ammo_box_amount, buf);
	if (level->game_logic.ammo_box)
		free(level->game_logic.ammo_box);
	level->game_logic.ammo_box = (t_item_pickup *)malloc(
			sizeof(t_item_pickup) * level->game_logic.ammo_box_amount);
	if (!level->game_logic.ammo_box)
		ft_error("memory allocation failed (deserialize level, ammo)");
	i = -1;
	while (++i < level->game_logic.ammo_box_amount)
	{
		deserialize_vec3(&level->game_logic.ammo_box[i].pos, buf);
		level->game_logic.ammo_box[i].start_time = 0;
		level->game_logic.ammo_box[i].visible = 1;
	}
}

void	deserialize_pickups(t_level *level, t_buffer *buf)
{
	deserialize_health_pickups(level, buf);
	deserialize_ammo_pickups(level, buf);
}

void	deserialize_enemies(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_enemy_settings(&level->game_logic.enemy_settings, buf);
	deserialize_int(&level->game_logic.enemy_amount, buf);
	if (level->game_logic.enemies)
		free(level->game_logic.enemies);
	level->game_logic.enemies = (t_enemy *)malloc(
			sizeof(t_enemy) * level->game_logic.enemy_amount);
	if (!level->game_logic.enemies)
		ft_error("memory allocation failed (deserialize level, enemies)");
	i = -1;
	while (++i < level->game_logic.enemy_amount)
		deserialize_vec3(&level->game_logic.enemies[i].spawn_pos, buf);
}

void	deserialize_menu_anim(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_int(&level->main_menu_anim.amount, buf);
	deserialize_int((int *)&level->main_menu_anim.duration, buf);
	deserialize_int(&level->main_menu_anim.loop, buf);
	if (level->main_menu_anim.pos)
		free(level->main_menu_anim.pos);
	level->main_menu_anim.pos = (t_player_pos *)malloc(
			sizeof(t_player_pos) * level->main_menu_anim.amount);
	if (!level->main_menu_anim.pos)
		ft_error("memory allocation failed (deserialize level, menu anim pos)");
	i = -1;
	while (++i < level->main_menu_anim.amount)
		deserialize_player_pos(&level->main_menu_anim.pos[i], buf);
}
