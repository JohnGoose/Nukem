/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_set_state_0.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 20:48:59 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_obj(t_level *level, char *filename)
{
	t_obj	tmp;

	if (!load_obj(filename, &tmp))
	{
		nonfatal_error("failed to read object file");
		return ;
	}
	delete_all_doors(level);
	delete_all_lights(level);
	free_culling(level);
	free(level->all.tris);
	level->all = tmp;
	if (!level->visible.tris)
		ft_error("memory allocation failed\n");
	init_culling(level);
}

void	set_texture(t_level *level, char *filename)
{
	t_bmp	tmp;

	tmp = bmp_read(filename);
	if (!tmp.image)
		return ;
	free(level->texture.image);
	free(level->baked);
	free(level->spray_overlay);
	level->texture = tmp;
	level->baked = (t_color *)malloc(sizeof(t_color)
			* (level->texture.width * level->texture.height));
	level->spray_overlay = (unsigned int *)malloc(sizeof(unsigned int)
			* (level->texture.width * level->texture.height));
	if (!level->spray_overlay || !level->baked)
		ft_error("memory allocation failed\n");
	ft_bzero(level->spray_overlay,
		level->texture.width * level->texture.height * 4);
	level->bake_status = BAKE_NOT_BAKED;
}

void	set_normal_map(t_level *level, char *filename)
{
	t_bmp	tmp;

	tmp = bmp_read(filename);
	if (!tmp.image)
		return ;
	free(level->normal_map.image);
	level->normal_map = tmp;
	level->bake_status = BAKE_NOT_BAKED;
}

void	set_skybox(t_level *level, char *filename)
{
	t_bmp	tmp;

	tmp = bmp_read(filename);
	if (!tmp.image)
		return ;
	free(level->sky.img.image);
	level->sky.img = tmp;
}

void	set_spray(t_level *level, char *filename)
{
	t_bmp	tmp;

	tmp = bmp_read(filename);
	if (!tmp.image)
		return ;
	free(level->spray.image);
	level->spray = tmp;
}
