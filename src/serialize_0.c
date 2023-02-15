/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	serialize_vec2(t_vec2 vec, t_buffer *buf)
{
	serialize_float(vec.x, buf);
	serialize_float(vec.y, buf);
}

void	serialize_vec3(t_vec3 vec, t_buffer *buf)
{
	serialize_float(vec.x, buf);
	serialize_float(vec.y, buf);
	serialize_float(vec.z, buf);
}

void	serialize_color(t_color_hsl color, t_buffer *buf)
{
	serialize_float(color.hue, buf);
	serialize_float(color.saturation, buf);
	serialize_float(color.lightness, buf);
	serialize_int(color.color, buf);
	serialize_float(color.r, buf);
	serialize_float(color.g, buf);
	serialize_float(color.b, buf);
}

void	serialize_settings(t_level *level, t_buffer *buf)
{
	serialize_int(level->ui.fog, buf);
	serialize_int(level->ui.fog_color.color, buf);
	serialize_int(level->ui.backface_culling, buf);
	serialize_int(level->ui.occlusion_culling, buf);
	serialize_int(level->ui.distance_culling, buf);
	serialize_float(level->ui.render_distance, buf);
	serialize_color(level->ui.sun_color, buf);
	serialize_vec3(level->ui.sun_dir, buf);
	serialize_vec3(level->game_logic.win_pos, buf);
	serialize_float(level->game_logic.win_dist, buf);
}

void	serialize_vert(t_vert *vert, t_buffer *buf)
{
	serialize_vec3(vert->pos, buf);
	serialize_vec2(vert->txtr, buf);
}
