/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 01:01:11 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	check_file_header(t_buffer *buf)
{
	char	*str;

	if (sizeof(int) * strlen("doom-nukem") > buf->size)
	{
		nonfatal_error("not valid doom-nukem map");
		return (1);
	}
	str = deserialize_string(ft_strlen("doom-nukem"), buf);
	if (ft_strcmp(str, "doom-nukem"))
	{
		nonfatal_error("not valid doom-nukem map");
		free(str);
		return (1);
	}
	free(str);
	return (0);
}

void	deserialize_lights(t_level *level, t_buffer *buf)
{
	int	i;

	deserialize_int(&level->light_amount, buf);
	if (level->lights)
		free(level->lights);
	level->lights = (t_light *)malloc(sizeof(t_light) * level->light_amount);
	if (!level->lights)
		ft_error("memory allocation failed (deserialize_lights)");
	i = 0;
	while (i < level->light_amount)
	{
		deserialize_vec3(&level->lights[i].pos, buf);
		deserialize_color(&level->lights[i].color, buf);
		deserialize_float(&level->lights[i].radius, buf);
		deserialize_float(&level->lights[i].power, buf);
		i++;
	}
}

void	deserialize_level(t_level *level, t_buffer *buf)
{
	if (check_file_header(buf))
		return ;
	deserialize_settings(level, buf);
	deserialize_level_images(level, buf);
	free_culling(level);
	free(level->all.tris);
	deserialize_projectile(&level->game_logic.player_projectile_settings, buf);
	level->game_logic.player_projectile_settings.shot_by_player = TRUE;
	deserialize_projectile(&level->game_logic.enemy_projectile_settings, buf);
	level->game_logic.enemy_projectile_settings.shot_by_player = FALSE;
	deserialize_int(&level->ui.normal_map_disabled, buf);
	deserialize_obj(&level->all, buf);
	deserialize_doors(level, buf);
	deserialize_lights(level, buf);
	deserialize_float(&level->world_brightness, buf);
	deserialize_float(&level->skybox_brightness, buf);
	deserialize_player_pos(&level->game_logic.spawn_pos, buf);
	deserialize_pickups(level, buf);
	deserialize_enemies(level, buf);
	deserialize_menu_anim(level, buf);
	init_culling(level);
	level->level_initialized = TRUE;
}
