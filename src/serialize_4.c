/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 18:36:21 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	serialize_level(t_level *level, t_buffer *buf)
{
	serialize_string("doom-nukem", buf);
	serialize_settings(level, buf);
	serialize_level_images(level, buf);
	serialize_projectile(&level->game_logic.player_projectile_settings, buf);
	serialize_projectile(&level->game_logic.enemy_projectile_settings, buf);
	serialize_int(level->ui.normal_map_disabled, buf);
	serialize_obj(&level->all, buf);
	serialize_doors(level, buf);
	serialize_lights(level, buf);
	serialize_float(level->world_brightness, buf);
	serialize_float(level->skybox_brightness, buf);
	serialize_player_pos(&level->game_logic.spawn_pos, buf);
	serialize_pickups(level, buf);
	serialize_enemies(level, buf);
	serialize_menu_anim(level, buf);
}
