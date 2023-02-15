/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/09 15:07:14 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	init_window_struct(t_window **window)
{
	window[0]->sdl_window = SDL_CreateWindow("DOOM NUKEM",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			RES_X, RES_Y, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window[0]->sdl_window)
		ft_error("could not create window");
	window[0]->sdl_renderer
		= SDL_CreateRenderer(window[0]->sdl_window, -1, 0);
	if (!window[0]->sdl_renderer)
		ft_error("could not create renderer");
	window[0]->raster_texture = empty_texture(window[0]->sdl_renderer);
	SDL_SetTextureBlendMode(window[0]->raster_texture, SDL_BLENDMODE_BLEND);
	window[0]->texture = empty_texture(window[0]->sdl_renderer);
	window[0]->frame_buffer = NULL;
	window[0]->depth_buffer
		= (float *)malloc(sizeof(float) * (RES_X * RES_Y));
	window[0]->pixel_pos_buffer
		= (t_vec3 *)malloc(sizeof(t_vec3) * (RES_X * RES_Y));
	window[0]->brightness_buffer
		= (t_color *)malloc(sizeof(t_color) * (RES_X * RES_Y));
	memset((void *)window[0]->brightness_buffer, 0,
		sizeof(t_color) * (RES_X * RES_Y));
	window[0]->post_process_buf
		= (unsigned int *)malloc(sizeof(unsigned int) * (RES_X * RES_Y));
	check_buffers(*window);
}

void	init_window(t_window **window)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		ft_error("could not initialize SDL\n");
	*window = (t_window *)malloc(sizeof(t_window));
	if (!*window)
		ft_error("memory allocation failed\n");
	init_window_struct(window);
	SDL_RenderClear((*window)->sdl_renderer);
}

void	init_audio_effects_2(t_level *l)
{
	unsigned char	*mem;
	SDL_RWops		*rw;

	mem = read_embedded_file("death.ogg");
	rw = SDL_RWFromMem(mem,
			(int)get_embedded_size("death.ogg") - 1);
	l->audio.death_mem = mem;
	l->audio.death = Mix_LoadWAV_RW(rw, 1);
	mem = read_embedded_file("jump.wav");
	rw = SDL_RWFromMem(mem,
			(int)get_embedded_size("jump.wav") - 1);
	l->audio.jump_mem = mem;
	l->audio.jump = Mix_LoadWAV_RW(rw, 1);
	mem = read_embedded_file("gunshot.wav");
	rw = SDL_RWFromMem(mem,
			(int)get_embedded_size("gunshot.wav") - 1);
	l->audio.gunshot_mem = mem;
	l->audio.gunshot = Mix_LoadWAV_RW(rw, 1);
}

void	init_audio_effects(t_level *l)
{
	unsigned char	*mem;
	SDL_RWops		*rw;

	init_audio_effects_2(l);
	mem = read_embedded_file("reload.wav");
	rw = SDL_RWFromMem(mem,
			(int)get_embedded_size("reload.wav") - 1);
	l->audio.reload_mem = mem;
	l->audio.reload = Mix_LoadWAV_RW(rw, 1);
	mem = read_embedded_file("door.wav");
	rw = SDL_RWFromMem(mem,
			(int)get_embedded_size("door.wav") - 1);
	l->audio.door_mem = mem;
	l->audio.door = Mix_LoadWAV_RW(rw, 1);
	if (!l->audio.reload || !l->audio.gunshot || !l->audio.jump
		|| !l->audio.death || !l->audio.door)
		ft_error("audio effects init error");
}

void	init_audio(t_level *l)
{
	unsigned char	*mem;
	SDL_RWops		*rw;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 8, 4096) < 0)
	{
		nonfatal_error("Failed to initialize audio");
		return ;
	}
	l->audio.music_volume = MIX_MAX_VOLUME / 10;
	l->audio.sound_effect_volume = MIX_MAX_VOLUME / 5;
	Mix_VolumeMusic(l->audio.music_volume);
	Mix_Volume(-1, l->audio.sound_effect_volume);
	mem = read_embedded_file("main_menu.ogg");
	rw = SDL_RWFromMem(mem, (int)get_embedded_size("main_menu.ogg") - 1);
	l->audio.title_music_mem = mem;
	l->audio.title_music = Mix_LoadMUS_RW(rw, 1);
	mem = read_embedded_file("ingame.ogg");
	rw = SDL_RWFromMem(mem, (int)get_embedded_size("ingame.ogg") - 1);
	l->audio.game_music_mem = mem;
	l->audio.game_music = Mix_LoadMUS_RW(rw, 1);
	if (!l->audio.game_music || !l->audio.title_music)
		ft_error("audio init error");
	init_audio_effects(l);
}
