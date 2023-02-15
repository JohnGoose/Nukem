/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/01 14:28:12 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	update_camera(t_level *l)
{
	l->cam.lon = -l->cam.look_side + M_PI / 2;
	l->cam.lat = -l->cam.look_up;
	rot_cam(&l->cam.front, l->cam.lon, l->cam.lat);
	rot_cam(&l->cam.up, l->cam.lon, l->cam.lat + (M_PI / 2));
	vec_cross(&l->cam.side, l->cam.up, l->cam.front);
	l->cam.fov_y = tanf(l->ui.fov / 2);
	l->cam.fov_x = tanf(l->ui.fov / 2) * ((float)RES_X / RES_Y);
}

void	render_raster(t_window *window, t_level *level)
{
	if (SDL_LockTexture(window->raster_texture, NULL,
			(void **)&window->raster_texture_pixels, &(int){0}) != 0)
		ft_error("failed to lock texture\n");
	memset(window->raster_texture_pixels, 0, RES_X * RES_Y * sizeof(int));
	wireframe(window->raster_texture_pixels, level);
	if (level->ui.state.gizmo_active)
		gizmo_render(level, window->raster_texture_pixels);
	SDL_UnlockTexture(window->raster_texture);
	SDL_RenderCopy(window->sdl_renderer, window->raster_texture, NULL, NULL);
}

void	render_ui(t_window *window, t_level *level,
						t_game_state *game_state)
{
	int	dummy_for_sdl;

	if (level->ui_hidden)
		return ;
	if (SDL_LockTexture(window->ui_texture, NULL,
			(void **)&window->ui_texture_pixels, &dummy_for_sdl) != 0)
		ft_error("failed to lock texture\n");
	memset(window->ui_texture_pixels, 0, RES_X * RES_Y * 4);
	ui(window, level, game_state);
	SDL_UnlockTexture(window->ui_texture);
	SDL_RenderCopy(window->sdl_renderer, window->ui_texture, NULL, NULL);
	SDL_RenderCopy(window->sdl_renderer, window->text_texture, NULL, NULL);
	SDL_SetRenderTarget(window->sdl_renderer, window->text_texture);
	SDL_RenderClear(window->sdl_renderer);
	SDL_RenderPresent(window->sdl_renderer);
	SDL_SetRenderTarget(window->sdl_renderer, NULL);
}

void	raycast_finish(t_window *window, t_level *level)
{
	level->ui.post_time = SDL_GetTicks();
	post_process(window, level);
	level->ui.post_time = SDL_GetTicks() - level->ui.post_time;
	SDL_UnlockTexture(window->texture);
	SDL_RenderCopy(window->sdl_renderer, window->texture, NULL, NULL);
}

void	render(t_window *window, t_level *level, t_game_state *game_state)
{
	unsigned int	raster_time;
	unsigned int	ui_time;

	if (level->level_initialized)
	{
		level->ui.total_raycasts = 0;
		level->ui.raycast_time = SDL_GetTicks();
		if (!level->ui.wireframe
			|| (level->ui.wireframe && level->ui.wireframe_on_top))
		{
			render_raycast(window, level, game_state);
			raycast_finish(window, level);
		}
		level->ui.raycast_time = SDL_GetTicks()
			- level->ui.raycast_time - level->ui.post_time;
		raster_time = SDL_GetTicks();
		if (*game_state == GAME_STATE_EDITOR)
			render_raster(window, level);
		level->ui.raster_time = SDL_GetTicks() - raster_time;
	}
	ui_time = SDL_GetTicks();
	render_ui(window, level, game_state);
	level->ui.ui_time = SDL_GetTicks() - ui_time;
	SDL_RenderPresent(window->sdl_renderer);
	SDL_RenderClear(window->sdl_renderer);
}
