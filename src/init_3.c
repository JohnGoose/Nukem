/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 14:09:54 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/09 19:57:02 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	check_buffers(t_window *window)
{
	if (!window->depth_buffer
		|| !window->post_process_buf
		|| !window->pixel_pos_buffer)
		ft_error("init window memory allocation failed\n");
}

void	init_embedded(t_level *level)
{
	unsigned char	*mem;

	mem = read_embedded_file("skybox.obj");
	load_obj_from_memory(mem, &level->sky.all);
	free(mem);
	mem = read_embedded_file("skybox.obj");
	load_obj_from_memory(mem, &level->sky.visible);
	free(mem);
	mem = read_embedded_file("pickup_box.obj");
	load_obj_from_memory(mem, &level->game_models.ammo_pickup_box);
	free(mem);
	mem = read_embedded_file("pickup_box.obj");
	load_obj_from_memory(mem, &level->game_models.health_pickup_box);
	free(mem);
	init_animation(level);
	init_textures(level);
	init_fonts(&level->ui);
}

void	realloc_baked_and_spray(t_level *level)
{
	if (level->baked)
		free(level->baked);
	level->baked = (t_color *)malloc(sizeof(t_color)
			* (level->texture.width * level->texture.height));
	if (!level->baked)
		ft_error("memory allocation failed\n");
	if (level->spray_overlay)
		free(level->spray_overlay);
	level->spray_overlay = (unsigned int *)malloc(sizeof(unsigned int)
			* (level->texture.width * level->texture.height));
	if (!level->spray_overlay)
		ft_error("memory allocation failed\n");
	ft_bzero(level->spray_overlay,
		level->texture.width * level->texture.height * 4);
	level->bake_status = BAKE_NOT_BAKED;
}
