/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/02 19:59:24 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_shader_settings(t_level *level)
{
	int		i;
	t_tri	*tri;

	if (call("close", NULL))
		level->ui.state.ui_location = UI_LOCATION_MAIN;
	i = -1;
	while (++i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
		{
			tri = &level->all.tris[i];
			choose_shader(tri);
			if (tri->shader == SHADER_PERLIN && tri->perlin)
				ui_config_face_perlin(tri->perlin, level);
			return ;
		}
	}
	level->ui.state.ui_location = UI_LOCATION_MAIN;
}

void	ui_level_settings(t_level *level)
{
	char	buf[100];

	call("edit lights", &enable_light_editor);
	file_browser("select obj", ".obj", &set_obj);
	file_browser("select texture", ".bmp", &set_texture);
	file_browser("select skybox", ".bmp", &set_skybox);
	file_browser("select normal map", ".bmp", &set_normal_map);
	button(&level->ui.normal_map_disabled, "disable normal map");
	call("add face", &add_face);
	if (call("game settings", NULL))
	{
		level->ui.state.ui_location = UI_LOCATION_GAME_SETTINGS;
		level->ui.state.logic_selected = GAME_LOGIC_SELECTED_NONE;
	}
	button(&level->ui.fog, "fog");
	if (level->ui.fog)
		color_slider(&level->ui.fog_color, NULL);
	button(&level->ui.backface_culling, "backface culling");
	if (level->ui.backface_culling)
		button(&level->ui.occlusion_culling,
			"occlusion culling (O(n^2))");
	button(&level->ui.distance_culling, "distance culling");
	ft_sprintf(buf, "render distance: %.1fm", level->ui.render_distance);
	float_slider(&level->ui.render_distance, buf, 2, 70);
	file_save("save level", ".doom-nukem", NULL);
}

void	ui_editor(t_level *level)
{
	t_editor_ui	*ui;

	ui = &level->ui;
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	button(&ui->noclip, "noclip");
	button(&ui->wireframe, "wireframe");
	if (ui->wireframe)
	{
		button(&ui->wireframe_on_top, "wireframe on top");
		button(&ui->show_quads, "quad visualize");
		button(&ui->wireframe_culling_visual, "culling visualize");
	}
	button(&ui->raytracing, "raytrace lights");
	button(&ui->vertex_select_mode, "vertex select mode");
	if (call("post process settings", NULL))
		level->ui.state.ui_location = UI_LOCATION_POST_PROCESS_SETTINGS;
	call("cull static", &static_culling);
	call("edit uv", &enable_uv_editor);
	call("edit doors", &enable_door_editor);
	if (nothing_selected(level) && level->bake_status != BAKE_BAKING)
		ui_level_settings(level);
	ui_config_selected_faces(level);
	set_text_color(UI_INFO_TEXT_COLOR);
	ui_render_info(&level->ui, level);
	ui_physics_info(&level->ui, level);
}

void	ui_baking(t_level *level)
{
	char	buf[100];

	if (level->bake_progress == 100.0)
		ft_sprintf(buf, "blurring lightmap");
	else
		ft_sprintf(buf, "baking: %.3f%%", level->bake_progress);
	set_text_color(UI_LEVEL_BAKING_COLOR);
	if (call(buf, NULL))
		level->bake_status = BAKE_NOT_BAKED;
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	button(&level->ui.noclip, "noclip");
	button(&level->ui.wireframe, "wireframe");
}

int	editor_select(t_level *level)
{
	if (level->ui.state.ui_location == UI_LOCATION_UV_EDITOR)
	{
		if (call("close uv editor", NULL))
			level->ui.state.ui_location = UI_LOCATION_MAIN;
		call("fix selected uv overlap", &fix_uv_overlap);
	}
	else if (level->ui.state.ui_location == UI_LOCATION_DOOR_EDITOR)
		ui_door_editor(level);
	else if (level->ui.state.ui_location == UI_LOCATION_DOOR_ACTIVATION_BUTTON)
	{
		if (call("return to door editor", NULL))
			level->ui.state.ui_location = UI_LOCATION_DOOR_EDITOR;
	}
	else if (level->ui.state.ui_location == UI_LOCATION_LIGHT_EDITOR)
		ui_light_editor(level);
	else if (level->ui.state.ui_location == UI_LOCATION_GAME_SETTINGS)
		ui_game_settings(level);
	else if (level->ui.state.ui_location
		== UI_LOCATION_ENEMY_AND_DAMAGE_SETTINGS)
		ui_enemy_and_damage_settings(level);
	else if (level->ui.state.ui_location == UI_LOCATION_SHADER_EDITOR)
		ui_shader_settings(level);
	else
		return (TRUE);
	return (FALSE);
}
