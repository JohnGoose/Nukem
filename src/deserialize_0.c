/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deserialize_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	deserialize_vec2(t_vec2 *vec, t_buffer *buf)
{
	deserialize_float(&vec->x, buf);
	deserialize_float(&vec->y, buf);
}

void	deserialize_vec3(t_vec3 *vec, t_buffer *buf)
{
	deserialize_float(&vec->x, buf);
	deserialize_float(&vec->y, buf);
	deserialize_float(&vec->z, buf);
}

void	deserialize_color(t_color_hsl *color, t_buffer *buf)
{
	deserialize_float(&color->hue, buf);
	deserialize_float(&color->saturation, buf);
	deserialize_float(&color->lightness, buf);
	deserialize_int(&color->color, buf);
	deserialize_float(&color->r, buf);
	deserialize_float(&color->g, buf);
	deserialize_float(&color->b, buf);
}

void	deserialize_settings(t_level *level, t_buffer *buf)
{
	deserialize_int(&level->ui.fog, buf);
	deserialize_int((int *)&level->ui.fog_color.color, buf);
	deserialize_int(&level->ui.backface_culling, buf);
	deserialize_int(&level->ui.occlusion_culling, buf);
	deserialize_int(&level->ui.distance_culling, buf);
	deserialize_float(&level->ui.render_distance, buf);
	deserialize_color(&level->ui.sun_color, buf);
	deserialize_vec3(&level->ui.sun_dir, buf);
	deserialize_vec3(&level->game_logic.win_pos, buf);
	deserialize_float(&level->game_logic.win_dist, buf);
}

void	deserialize_vert(t_vert *vert, t_buffer *buf)
{
	deserialize_vec3(&vert->pos, buf);
	deserialize_vec2(&vert->txtr, buf);
}
