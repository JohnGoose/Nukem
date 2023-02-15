/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_post_process_settings(t_level *level)
{
	char		buf[100];
	t_editor_ui	*ui;

	ui = &level->ui;
	ui_post_process_go_back(level);
	set_text_color(UI_POST_PROCESS_OTHER);
	int_slider(&ui->chromatic_abberation, "chroma (20ms expensive)", 0, 30);
	float_slider(&ui->sharpen, "sharpen (60ms very expensive)", 0.0, 5.0);
	button(&ui->smooth_pixels, "smooth pixel (20ms expensive)");
	button(&ui->blur, "blur (1ms cheap)");
	set_text_color(UI_POST_PROCESS_BLOOM);
	ft_sprintf(buf, "bloom radius: %.1f pixels", level->ui.bloom_radius);
	float_slider(&level->ui.bloom_radius, buf, 0, 100);
	ft_sprintf(buf, "bloom intensity: %.1f", level->ui.bloom_intensity);
	float_slider(&level->ui.bloom_intensity, buf, 0, 5);
	ft_sprintf(buf, "bloom limit: %.1f", level->ui.bloom_limit);
	float_slider(&level->ui.bloom_limit, buf, 0, 5);
	set_text_color(UI_POST_PROCESS_SSAO);
	int_slider(&level->ui.ssao_radius, "ssao radius", 0, 40);
	float_slider(&level->ui.ssao_intensity, "ssao intensity", 1, 10);
	float_slider(&level->ui.ssao_light_bias, "ssao light bias", 0, 15);
	set_text_color(UI_POST_PROCESS_DEBUG);
	button(&level->ui.bloom_debug, "render bloom only");
	button(&level->ui.ssao_debug, "render ssao only");
	button(&ui->state.ssp_visual, "ssp visualize");
}

void	ui_render_settings(t_level *level)
{
	char		buf[100];
	t_editor_ui	*ui;
	float		fov_angle;

	ui = &level->ui;
	ft_sprintf(buf, "render scale: %d (%.0f%%)", ui->raycast_quality,
		100.0 / (float)ui->raycast_quality);
	int_slider(&ui->raycast_quality, buf, 1, 20);
	fov_angle = ui->fov;
	fov_angle *= 180.0 / M_PI;
	ft_sprintf(buf, "fov: %d", (int)fov_angle);
	float_slider(&ui->fov, buf, M_PI / 6, M_PI);
}

void	ui_settings_volume(t_level *level)
{
	char		buf[100];

	ft_sprintf(buf, "music volume: %.0f%%",
		100 * (level->audio.music_volume / MIX_MAX_VOLUME));
	float_slider(&level->audio.music_volume, buf, 0, MIX_MAX_VOLUME);
	Mix_VolumeMusic(level->audio.music_volume);
	ft_sprintf(buf, "sound effect volume: %.0f%%",
		100 * (level->audio.sound_effect_volume / MIX_MAX_VOLUME));
	float_slider(&level->audio.sound_effect_volume, buf, 0, MIX_MAX_VOLUME);
	Mix_Volume(-1, level->audio.sound_effect_volume);
}

void	ui_settings(t_level *level)
{
	char		buf[100];

	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	if (call("close", NULL))
		level->ui.main_menu = MAIN_MENU_LOCATION_MAIN;
	if (call("select spray", NULL))
	{
		level->ui.state.ui_location = UI_LOCATION_FILE_OPEN;
		level->ui.main_menu = MAIN_MENU_LOCATION_SPRAY_SELECT;
		ft_strcpy(level->ui.state.extension, ".bmp");
		level->ui.state.open_file = &set_spray;
	}
	button(&level->ui.spray_from_view, "spray from view");
	if (!level->ui.spray_from_view)
		ft_sprintf(buf, "spray size: %.1f", level->ui.spray_size);
	if (!level->ui.spray_from_view)
		float_slider(&level->ui.spray_size, buf, 0.1, 5);
	ui_render_settings(level);
	ui_settings_volume(level);
	if (call("post process settings", NULL))
		level->ui.main_menu = MAIN_MENU_LOCATION_POST_PROCESS;
}

void	ui_door_settings(t_level *level)
{
	char	buf[100];

	call("set door start position", &set_door_pos_1);
	call("set door stop position", &set_door_pos_2);
	if (button(&level->doors.door[level->doors.selected_index - 1]
			.is_activation_pos_active, "has activation button"))
	{
		level->doors.door[level->doors.selected_index - 1]
			.activation_pos = level->cam.pos;
		vec_add(
			&level->doors.door[level->doors.selected_index - 1]
			.activation_pos,
			level->doors.door[level->doors.selected_index - 1]
			.activation_pos, level->cam.front);
	}
	if (level->doors.door[level->doors.selected_index - 1]
		.is_activation_pos_active)
		if (call("move door activation button", NULL))
			level->ui.state.ui_location
				= UI_LOCATION_DOOR_ACTIVATION_BUTTON;
	ft_sprintf(buf, "door transition time: %fs",
		level->doors.door[level->doors.selected_index - 1]
		.transition_time);
	float_slider(&level->doors.door[level->doors.selected_index - 1]
		.transition_time, buf, .2, 7);
}
