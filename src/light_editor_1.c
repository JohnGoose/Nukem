/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_editor_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	add_light(t_level *level)
{
	level->bake_status = BAKE_NOT_BAKED;
	level->light_amount++;
	level->lights = (t_light *)ft_realloc(level->lights,
			sizeof(t_light) * level->light_amount - 1,
			sizeof(t_light) * level->light_amount);
	if (!level->lights)
		ft_error("memory allocation failed\n");
	vec_add(&level->lights[level->light_amount - 1].pos,
		level->cam.pos,
		level->cam.front);
	level->lights[level->light_amount - 1].color.hue = 1;
	level->lights[level->light_amount - 1].color.saturation = 1;
	level->lights[level->light_amount - 1].color.lightness = 1;
	hsl_update_color(&level->lights[level->light_amount - 1].color);
	level->lights[level->light_amount - 1].radius = 10;
	level->lights[level->light_amount - 1].power = 1;
	level->bake_status = BAKE_NOT_BAKED;
}

void	enable_light_editor(t_level *level)
{
	level->ui.state.ui_location = UI_LOCATION_LIGHT_EDITOR;
	level->ui.vertex_select_mode = FALSE;
	deselect_all_faces(level);
}
