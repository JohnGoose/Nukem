/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

unsigned int	crossfade(unsigned int color1, unsigned int color2,
							unsigned int fade, unsigned int alpha)
{
	unsigned char	*rgb1;
	unsigned char	*rgb2;
	unsigned int	newr;
	unsigned int	newg;
	unsigned int	newb;

	rgb1 = (unsigned char *)&color1;
	rgb2 = (unsigned char *)&color2;
	newr = (rgb1[2] * (0xff - fade) + rgb2[2] * fade) / 0xff;
	newg = (rgb1[1] * (0xff - fade) + rgb2[1] * fade) / 0xff;
	newb = (rgb1[0] * (0xff - fade) + rgb2[0] * fade) / 0xff;
	return ((newr << 8 * 3) + (newg << 8 * 2) + (newb << 8 * 1) + alpha);
}

unsigned int	brightness(unsigned int color1, t_color new)
{
	unsigned char	*rgb;
	unsigned int	newr;
	unsigned int	newg;
	unsigned int	newb;

	rgb = (unsigned char *)&color1;
	newr = rgb[2] * sqrt(new.r);
	newg = rgb[1] * sqrt(new.g);
	newb = rgb[0] * sqrt(new.b);
	if (newr > 0xff)
		newr = 0xff;
	if (newg > 0xff)
		newg = 0xff;
	if (newb > 0xff)
		newb = 0xff;
	return ((newr << 8 * 3) + (newg << 8 * 2) + (newb << 8 * 1));
}

t_color	get_skybox_brightness(t_level *l)
{
	t_color	tmp;

	if (l->skybox_brightness != 0)
	{
		tmp.r = l->skybox_brightness;
		tmp.g = l->skybox_brightness;
		tmp.b = l->skybox_brightness;
	}
	else
	{
		tmp.r = l->ui.sun_color.r + l->world_brightness;
		tmp.g = l->ui.sun_color.g + l->world_brightness;
		tmp.b = l->ui.sun_color.b + l->world_brightness;
	}
	return (tmp);
}

void	skybox_reset_result(t_level *l, t_cast_result *res)
{
	res->ray.pos = (t_vec3){0, 0, 0};
	res->texture = &l->sky.img;
	res->normal_map = NULL;
	res->baked = NULL;
	res->spray_overlay = NULL;
	res->raytracing = 0;
}

float	skybox_sun_fade(t_color *col, t_cast_result *res, t_level *l)
{
	float	fade;

	if (!l->ui.sun_color.r && !l->ui.sun_color.g && !l->ui.sun_color.b)
		return (0);
	fade = vec_dot(l->ui.sun_dir, res->ray.dir);
	if (fade < SUN_SIZE)
		return (0);
	fade = fade - SUN_SIZE;
	fade = fade / (1.0 - SUN_SIZE);
	fade = (sin(fade * M_PI - M_PI / 2) + 1) / 2;
	col->r = l->ui.sun_color.r;
	col->g = l->ui.sun_color.g;
	col->b = l->ui.sun_color.b;
	res->light.r += col->r;
	res->light.g += col->g;
	res->light.b += col->b;
	return (fade);
}
