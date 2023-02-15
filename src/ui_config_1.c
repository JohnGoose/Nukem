/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_enemy_and_damage_settings(t_level *level)
{
	if (call("close", NULL))
		level->ui.state.ui_location = UI_LOCATION_GAME_SETTINGS;
	set_text_color(UI_INFO_TEXT_COLOR);
	text("enemy settings");
	ui_config_enemy_settings(&level->game_logic.enemy_settings);
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	text("enemy projectile settings");
	ui_config_projectile_settings(&level->game_logic.enemy_projectile_settings);
	set_text_color(UI_LEVEL_BAKED_COLOR);
	text("player projectile settings");
	ui_config_projectile_settings(
		&level->game_logic.player_projectile_settings);
}

void	ui_config_face_perlin_settings(t_perlin_settings *p)
{
	char	buf[100];

	float_slider(&p->min, "min", 0, 1);
	float_slider(&p->max, "max", 0, 1);
	float_slider(&p->noise_opacity, "opacity from noise", 0, 4);
	float_slider(&p->distance, "distance (0 = disabled)", 0, 800);
	int_slider(&p->resolution, "resolution", 1, 6);
	float_slider(&p->depth, "depth", 0, 5);
	float_slider(&p->scale, "scale", 0.01, 2);
	float_slider(&p->move_speed, "speed", 0, 3);
	ft_sprintf(buf, "depth speed difference: %.2f", p->speed_diff);
	float_slider(&p->speed_diff, buf, 0, 4);
	int_slider(&p->visualizer, "visualizer", 0, 3);
	if (color_slider(&p->color_1, "color 1"))
		hsl_update_color(&p->color_1);
	if (color_slider(&p->color_2, "color 2"))
		hsl_update_color(&p->color_2);
}

void	ui_config_face_perlin(t_perlin_settings *perlin, t_level *level)
{
	set_text_color(UI_SHADER_SETTINGS);
	float_slider(&perlin->swirl, "swirl", 0, 5);
	if (perlin->swirl != 0)
	{
		float_slider(&perlin->swirl_interval, "swirl interval", 0.0001, 20);
		if (call("set swirl position", NULL))
		{
			perlin->dir.x = level->cam.pos.x;
			perlin->dir.y = level->cam.pos.z;
		}
	}
	else
	{
		if (call("set noise direction", NULL))
		{
			perlin->dir.x = level->cam.front.x;
			perlin->dir.y = level->cam.front.z;
		}
		vec2_normalize(&perlin->dir);
	}
	ui_config_face_perlin_settings(perlin);
}

void	ui_confing_face_render_settings(t_tri *tri, t_level *level)
{
	char	buf[100];

	ft_sprintf(buf, "opacity: %.0f%%",
		100 * tri->opacity);
	float_slider(&tri->opacity, buf, 0, 1);
	if (tri->opacity)
	{
		button(&tri->opacity_precise, "precise opacity");
		ft_sprintf(buf, "refractive index: %.2f",
			tri->refractivity);
		float_slider(&tri->refractivity, buf, -1, 3);
	}
	if (button(&tri->isquad, "quad"))
		set_fourth_vertex(tri);
	button(&tri->isgrid, "grid");
	button(&tri->isbreakable, "breakable");
	if (call("shader editor", NULL))
		level->ui.state.ui_location = UI_LOCATION_SHADER_EDITOR;
}

void	ui_confing_face_settings(t_level *level,
							int selected_amount, t_tri *tri)
{
	char	buf[100];

	if (selected_amount == 1)
		text("Selected face:");
	else
	{
		ft_sprintf(buf, "%d faces selected:", selected_amount);
		text(buf);
	}
	if (call("remove faces", &remove_faces))
		return ;
	if (!tri->reflectivity || selected_amount != 1)
		ft_sprintf(buf, "reflectivity: %.0f%%",
			100 * tri->reflectivity);
	else
		ft_sprintf(buf,
			"reflectivity: %.0f%% (%d mirror %d first bounce)",
			100 * tri->reflectivity,
			tri->reflection_obj_all.tri_amount,
			tri->reflection_obj_first_bounce.tri_amount);
	if (float_slider(&tri->reflectivity, buf, 0, 1))
		static_culling(level);
	ui_confing_face_render_settings(tri, level);
}
