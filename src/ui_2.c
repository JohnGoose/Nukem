/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/16 12:43:13 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	init_ui_state(t_level *level)
{
	level->ui.state.extension = (char *)malloc(sizeof(char) * NAME_MAX);
	if (!level->ui.state.extension)
		ft_error("memory allocation failed\n");
	level->ui.state.save_filename = (char *)malloc(sizeof(char) * NAME_MAX);
	if (!level->ui.state.save_filename)
		ft_error("memory allocation failed\n");
	memset(level->ui.state.extension, 0, NAME_MAX - 1);
	memset(level->ui.state.save_filename, 0, NAME_MAX - 1);
	level->ui.state.uv_pos.x = 0;
	level->ui.state.uv_pos.y = 0;
	level->ui.state.uv_zoom = 1;
	level->ui.state.directory = get_current_directory();
	chdir(level->ui.state.directory);
}

void	init_ui_settings_post_process(t_editor_ui *ui)
{
	ui->blur = FALSE;
	ui->smooth_pixels = FALSE;
	ui->bloom_intensity = 0;
	ui->bloom_radius = 40;
	ui->bloom_limit = 0;
	ui->bloom_debug = FALSE;
	ui->ssao_radius = 0;
	ui->ssao_intensity = 1;
	ui->ssao_debug = 0;
}

void	init_ui_settings(t_editor_ui *ui)
{
	ui->noclip = TRUE;
	ui->backface_culling = TRUE;
	ui->occlusion_culling = FALSE;
	ui->distance_culling = FALSE;
	ui->wireframe = FALSE;
	ui->wireframe_on_top = TRUE;
	ui->wireframe_culling_visual = TRUE;
	ui->render_distance = 20;
	ui->raycast_quality = NOISE_QUALITY_LIMIT - 1;
	ui->fov = M_PI / 2;
	ui->sun_color.r = 1;
	ui->sun_color.g = 1;
	ui->sun_color.b = 1;
	ui->sun_dir = (t_vec3){1, -1, 1};
	vec_normalize(&ui->sun_dir);
	ui->spray_from_view = 1;
	ui->spray_size = 3;
	ui->fog_color.hue = 1;
	ui->fog_color.saturation = 1;
	ui->fog_color.lightness = 0;
	ui->fog_color.color = 0xffffffff;
	ui->normal_map_disabled = FALSE;
	init_ui_settings_post_process(ui);
}

void	init_color_slider(t_level *level)
{
	unsigned int	i;
	unsigned int	red;
	unsigned int	grn;
	unsigned int	blu;
	float			pos;

	level->ui.state.color_slider_hue_colors = (unsigned int *)malloc
		(sizeof(unsigned int) * UI_SLIDER_WIDTH);
	if (!level->ui.state.color_slider_hue_colors)
		ft_error("color slider memory allocation failed");
	i = 0;
	while (i < UI_SLIDER_WIDTH)
	{
		pos = i / (float)UI_SLIDER_WIDTH * M_PI * 2;
		red = 0xff * ((sin(pos + (M_PI * 2 * ((1.0 / 3) * 1))) + 1) / 2);
		grn = 0xff * ((sin(pos + (M_PI * 2 * ((1.0 / 3) * 2))) + 1) / 2);
		blu = 0xff * ((sin(pos + (M_PI * 2 * ((1.0 / 3) * 3))) + 1) / 2);
		red = red << 8 * 3;
		grn = grn << 8 * 2;
		blu = blu << 8 * 1;
		level->ui.state.color_slider_hue_colors[i] = red + grn + blu + 0xff;
		i++;
	}
}

void	init_ui(t_window *window, t_level *level)
{
	int	width;

	window->text_texture = SDL_CreateTexture(window->sdl_renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RES_X, RES_Y);
	window->ui_texture = SDL_CreateTexture(window->sdl_renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
			RES_X, RES_Y);
	init_ui_settings(&level->ui);
	init_ui_state(level);
	SDL_SetTextureBlendMode(window->text_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(window->ui_texture, SDL_BLENDMODE_BLEND);
	if (SDL_LockTexture(window->ui_texture, NULL,
			(void **)&window->ui_texture_pixels, &width) != 0)
		ft_error("failed to lock texture\n");
	get_ui_state(&level->ui.state);
	get_window(window);
	get_level(level);
	init_color_slider(level);
}
