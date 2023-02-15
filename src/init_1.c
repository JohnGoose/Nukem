/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/09 19:59:55 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	projectile_default(t_projectile *projectile)
{
	projectile->speed = 10;
	projectile->dist = 1;
	projectile->damage = 10;
}

void	init_enemy_settings(t_enemy_settings *enemy)
{
	enemy->dist_limit = 30;
	enemy->move_speed = 2;
	enemy->initial_health = 100;
	enemy->melee_range = 1.5;
	enemy->attack_frequency = 500.0;
	enemy->move_duration = 3.0;
	enemy->shoot_duration = 2.0;
	enemy->melee_damage = 30;
}

void	level_default_settings(t_level *level)
{
	level->game_logic.player.health = PLAYER_HEALTH_MAX;
	level->game_logic.player.ammo = PLAYER_AMMO_MAX;
	level->game_logic.win_dist = INITIAL_LEVEL_WIN_DIST;
	level->cam.pos.x = 0;
	level->cam.pos.y = -PLAYER_EYE_HEIGHT;
	level->cam.pos.z = 0;
	level->cam.look_side = 0;
	level->cam.look_up = 0;
	level->main_menu_anim.duration = 2;
	level->world_brightness = 0.5;
	level->ui.sun_color.hue = 0.768421;
	level->ui.sun_color.saturation = 0.0;
	level->ui.sun_color.lightness = 0.936842;
	hsl_update_color(&level->ui.sun_color);
	level->game_logic.win_pos.x = 10;
	init_enemy_settings(&level->game_logic.enemy_settings);
	projectile_default(&level->game_logic.enemy_projectile_settings);
	level->game_logic.enemy_projectile_settings.shot_by_player = FALSE;
	projectile_default(&level->game_logic.player_projectile_settings);
	level->game_logic.player_projectile_settings.shot_by_player = TRUE;
}

void	read_embedded_images(t_level *level, unsigned char *mem)
{
	mem = read_embedded_file("normal.bmp");
	if (level->normal_map.image)
		free(level->normal_map.image);
	level->normal_map = bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("skybox.bmp");
	if (level->sky.img.image)
		free(level->sky.img.image);
	level->sky.img = bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("spray.bmp");
	if (level->spray.image)
		free(level->spray.image);
	level->spray = bmp_read_from_memory(mem);
	free(mem);
}

void	create_default_level(t_level *level)
{
	unsigned char	*mem;

	delete_all_doors(level);
	delete_all_lights(level);
	level_default_settings(level);
	mem = read_embedded_file("out.bmp");
	if (level->texture.image)
		free(level->texture.image);
	level->texture = bmp_read_from_memory(mem);
	free(mem);
	realloc_baked_and_spray(level);
	read_embedded_images(level, mem);
	mem = read_embedded_file("ship_final.obj");
	if (level->all.tris)
	{
		free_culling(level);
		free(level->all.tris);
	}
	load_obj_from_memory(mem, &level->all);
	free(mem);
	init_culling(level);
}
