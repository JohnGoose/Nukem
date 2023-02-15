/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/05 21:53:43 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

SDL_Texture	*empty_texture(SDL_Renderer *renderer)
{
	SDL_Texture	*texture;

	texture = NULL;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING, RES_X, RES_Y);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	return (texture);
}

void	init_fonts(t_editor_ui *ui)
{
	SDL_RWops		*rwmem;
	unsigned char	*mem;
	unsigned long	size;

	TTF_Init();
	mem = read_embedded_file("digital.ttf");
	size = get_embedded_size("digital.ttf") - 1;
	ui->digital_data = mem;
	rwmem = SDL_RWFromMem((void *)mem, size);
	ui->hud_font = TTF_OpenFontRW(rwmem, 1, HUD_FONT_SIZE);
	mem = read_embedded_file("roboto.ttf");
	size = get_embedded_size("roboto.ttf") - 1;
	ui->roboto_data = mem;
	rwmem = SDL_RWFromMem((void *)mem, size);
	ui->editor_font = TTF_OpenFontRW(rwmem, 1, UI_FONT_SIZE);
	rwmem = SDL_RWFromMem((void *)mem, size);
	ui->main_menu_font = TTF_OpenFontRW(rwmem, 1, MAIN_MENU_FONT_SIZE);
	rwmem = SDL_RWFromMem((void *)mem, size);
	ui->win_lose_font = TTF_OpenFontRW(rwmem, 1, HUD_GAME_EVENT_FONT_SIZE);
	if (!ui->editor_font || !ui->hud_font
		|| !ui->main_menu_font || !ui->win_lose_font)
		ft_error("font open fail");
}

void	init_animation(t_level *level)
{
	unsigned char	*mem;
	int				res;

	res = 0;
	mem = read_embedded_file("enemy_shoot.obj");
	load_obj_from_memory(mem, &level->game_models.enemy_shoot);
	free(mem);
	res += load_animation("_run",
			&level->game_models.enemy_run, 10, 1.0);
	level->game_models.enemy
		= get_animation_target(&level->game_models.enemy_run);
	res += load_animation("_die",
			&level->game_models.enemy_die, 14, 3.0);
	level->game_models.enemy_die.loop = FALSE;
	res += load_animation("odel", &level->game_models.reload_animation,
			11, RELOAD_ANIMATION_DURATION);
	level->game_models.viewmodel
		= get_animation_target(&level->game_models.reload_animation);
	if (res != 3)
		ft_error("animation read fail");
}

void	init_textures_2(t_level *level)
{
	unsigned char	*mem;

	mem = read_embedded_file("vulcan_diff.bmp");
	level->game_models.viewmodel.texture
		= bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("lightsprite.bmp");
	level->game_models.light_sprite
		= bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("projectile.bmp");
	level->game_models.projectile_sprite
		= bmp_read_from_memory(mem);
	free(mem);
}

void	init_textures(t_level *level)
{
	unsigned char	*mem;

	mem = read_embedded_file("title.bmp");
	level->main_menu_title = bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("ammo_diff.bmp");
	level->game_models.ammo_pickup_box.texture
		= bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("health_diff.bmp");
	level->game_models.health_pickup_box.texture
		= bmp_read_from_memory(mem);
	free(mem);
	mem = read_embedded_file("enemy_diff.bmp");
	level->game_models.enemy.texture
		= bmp_read_from_memory(mem);
	free(mem);
	init_textures_2(level);
}
