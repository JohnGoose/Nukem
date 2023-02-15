/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_editor_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	uv_wireframe_selected(t_level *level,
			t_uv_parameters param, t_ivec2 *mouse)
{
	int		i;

	i = -1;
	while (++i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
		{
			param.tri = &level->all.tris[i];
			draw_face_uv(level, param, *mouse);
		}
	}
}

void	uv_wireframe(t_level *level, t_ivec2 offset,
						unsigned int *pixels, float image_scale)
{
	t_ivec2			mouse;
	t_uv_parameters	param;

	mouse = level->ui.state.mouse;
	param.offset.x = offset.x;
	param.offset.y = offset.y;
	param.scale.x = level->texture.width * image_scale;
	param.scale.y = level->texture.height * image_scale;
	param.pixels = pixels;
	uv_wireframe_selected(level, param, &mouse);
	update_uv_closest_vertex(level, image_scale, offset, mouse);
}

void	draw_texture(t_level *level, unsigned int *pixels,
		float image_scale, t_ivec2 offset)
{
	int				x;
	int				y;
	unsigned int	address;

	y = -1;
	while (++y < RES_Y)
	{
		x = -1;
		while (++x < RES_X / 2)
		{
			if (x - offset.x >= 0
				&& x - offset.x < (int)(level->texture.width * image_scale)
				&& y - offset.y >= 0
				&& y - offset.y < (int)(level->texture.height * image_scale))
			{
				address = (y - offset.y) / image_scale;
				address *= level->texture.width;
				address += (int)((x - offset.x) / image_scale);
				uv_pixel_put(x, y, level->texture.image[address], pixels);
			}
		}
	}
}

void	uv_editor(t_level *level, unsigned int *pixels)
{
	float	image_scale;
	t_ivec2	offset;
	int		x;
	int		y;

	image_scale = get_texture_scale(&level->texture) * level->ui.state.uv_zoom;
	offset.x = level->ui.state.uv_pos.x;
	offset.y = (UI_ELEMENT_HEIGHT + UI_PADDING * 2) + level->ui.state.uv_pos.y;
	offset.x += RES_X / 4;
	offset.y += RES_Y / 2;
	offset.x -= level->texture.width * image_scale / 2;
	offset.y -= level->texture.height * image_scale / 2;
	y = -1;
	while (++y < RES_Y)
	{
		x = -1;
		while (++x < RES_X / 2)
			uv_pixel_put(x, y, UI_BACKGROUND_COL, pixels);
	}
	draw_texture(level, pixels, image_scale, offset);
	uv_wireframe(level, offset, pixels, image_scale);
}

void	enable_uv_editor(t_level *level)
{
	level->ui.state.ui_location = UI_LOCATION_UV_EDITOR;
	level->ui.wireframe = TRUE;
	level->ui.wireframe_on_top = TRUE;
}
