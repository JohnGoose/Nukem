/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bake_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	blur_bake(t_level *level)
{
	t_blur			blur;
	t_color			*buff;
	unsigned int	size;

	size = level->baked_size.x * level->baked_size.y;
	buff = (t_color *)malloc(sizeof(t_color) * size);
	if (!buff)
		return ;
	ft_memcpy(buff, level->baked, sizeof(t_color) * size);
	ft_memset(level->baked, 0, sizeof(t_color) * size);
	blur.buff = level->baked;
	blur.pixels = buff;
	blur.radius = level->ui.bake_blur_radius;
	blur.intensity = 1;
	blur.quality = 1;
	blur.size.x = level->baked_size.x;
	blur.size.y = level->baked_size.y;
	blur.skip_zeroes = TRUE;
	box_blur(blur, -1);
	free(buff);
}

int	bake(void *d)
{
	t_level			*l;
	t_cast_result	res;
	int				tri;

	l = d;
	tri = 0;
	cast_result_set(&res, l);
	res.raytracing = TRUE;
	while (tri < l->all.tri_amount)
	{
		res.face_index = tri;
		if (!l->all.tris[tri].isgrid && l->all.tris[tri].shader == SHADER_NONE)
			bake_face(&res, l);
		tri++;
		l->bake_progress = 100 * (float)tri / (float)l->all.tri_amount;
	}
	if (l->ui.bake_blur_radius > 1)
		blur_bake(l);
	if (l->bake_status != BAKE_NOT_BAKED)
		l->bake_status = BAKE_BAKED;
	return (1);
}

void	clear_bake(t_level *level)
{
	int	x;
	int	y;

	x = 0;
	while (x < level->texture.width)
	{
		y = 0;
		while (y < level->texture.height)
		{
			level->baked[x + y * level->texture.width].r = 0;
			level->baked[x + y * level->texture.width].g = 0;
			level->baked[x + y * level->texture.width].b = 0;
			y++;
		}
		x++;
	}
}

void	start_bake(t_level *level)
{
	SDL_Thread	*bake_thread;

	if ((level->texture.width != level->normal_map.width
			|| level->texture.height != level->normal_map.height)
		&& !level->ui.normal_map_disabled)
	{
		nonfatal_error("texture and normal map are not same size");
		return ;
	}
	if (level->baked_size.x != level->texture.width
		|| level->baked_size.y != level->texture.height)
		level->baked = ft_realloc(level->baked,
				sizeof(t_color) * level->baked_size.x * level->baked_size.y,
				sizeof(t_color) * level->texture.height * level->texture.width);
	if (level->bake_status == BAKE_NOT_BAKED)
	{
		clear_bake(level);
		level->bake_status = BAKE_BAKING;
		level->bake_progress = 0;
		bake_thread = SDL_CreateThread(bake, "bake", (void *)level);
		SDL_DetachThread(bake_thread);
		level->baked_size.x = level->texture.width;
		level->baked_size.y = level->texture.height;
	}
}
