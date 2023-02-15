/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 16:44:53 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_door_editor(t_level *level)
{
	int			selected;
	int			i;

	if (call("close door editor", NULL))
		level->ui.state.ui_location = UI_LOCATION_MAIN;
	find_selected_door_index(level);
	if (level->doors.selected_index)
		call("delete selected door", &delete_door);
	if (level->doors.selected_index)
		ui_door_settings(level);
	else
	{
		selected = 0;
		i = -1;
		while (++i < level->all.tri_amount)
			if (level->all.tris[i].selected)
				selected++;
		if (selected)
			call("new door from selection", &add_new_door);
		else
			text("Select faces to create door from");
	}
}

void	ui_single_light_settings(t_level *level)
{
	char	buf[100];
	int		changed;

	if (!level->selected_light_index)
		return ;
	set_text_color(level->lights[level->selected_light_index - 1].color.color);
	changed = 0;
	changed += color_slider(
			&level->lights[level->selected_light_index - 1].color,
			"light color");
	ft_sprintf(buf, "radius: %.2f",
		level->lights[level->selected_light_index - 1].radius);
	changed += float_slider(
			&level->lights[level->selected_light_index - 1].radius,
			buf, .1, 50);
	ft_sprintf(buf, "power: %.2f",
		level->lights[level->selected_light_index - 1].power);
	changed += float_slider(
			&level->lights[level->selected_light_index - 1].power,
			buf, .1, 5);
	call("delete light", &delete_light);
	if (changed)
		level->bake_status = BAKE_NOT_BAKED;
}

void	ui_level_light_settings(t_level *level)
{
	char	buf[100];
	int		changed;

	changed = 0;
	ft_sprintf(buf, "world brightness: %.2f", level->world_brightness);
	changed += float_slider(&level->world_brightness, buf, 0, 1);
	ft_sprintf(buf, "skybox brightness: %.2f (0 = sync)",
		level->skybox_brightness);
	changed += float_slider(&level->skybox_brightness, buf, 0, 1);
	changed += color_slider(&level->ui.sun_color, "sun color");
	ft_sprintf(buf, "sun dir: (%.2f, %.2f, %.2f)", level->ui.sun_dir.x,
		level->ui.sun_dir.y, level->ui.sun_dir.z);
	text(buf);
	changed += float_slider(&level->ui.sun_dir.x, NULL, -1, 1);
	changed += float_slider(&level->ui.sun_dir.y, NULL, -1, 1);
	changed += float_slider(&level->ui.sun_dir.z, NULL, -1, 1);
	if (changed)
	{
		vec_normalize(&level->ui.sun_dir);
		level->bake_status = BAKE_NOT_BAKED;
	}
}

void	ui_light_editor_bake(t_level *level)
{
	char		buf[100];

	set_text_color(UI_LEVEL_NOT_BAKED_COLOR);
	ft_sprintf(buf, "bake lighting");
	if (call(buf, start_bake))
		level->selected_light_index = 0;
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	ft_sprintf(buf, "bake scale: %d (%.0f%%)", level->ui.bake_quality,
		100.0 / (float)level->ui.bake_quality);
	int_slider(&level->ui.bake_quality, buf, 1, 10);
	ft_sprintf(buf, "bake blur radius: %d pixels", level->ui.bake_blur_radius);
	int_slider(&level->ui.bake_blur_radius, buf, 1, 10);
}

void	bake_buttons(t_level *level)
{
	char		buf[100];

	if (level->bake_status == BAKE_NOT_BAKED)
		ui_light_editor_bake(level);
	else if (level->bake_status == BAKE_BAKED)
	{
		set_text_color(UI_LEVEL_BAKED_COLOR);
		ft_sprintf(buf, "lighting baked");
		if (call(buf, NULL))
			level->bake_status = BAKE_NOT_BAKED;
	}
}
