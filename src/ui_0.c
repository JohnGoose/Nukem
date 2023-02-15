/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_0.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/03/09 15:06:30 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	button_pixel_put(int x, int y, int color, unsigned int *texture)
{
	if (x < 0 || y < 0 || x >= RES_X || y >= RES_Y)
		return ;
	texture[x + (y * RES_X)] = color;
}

void	set_text_color(int color)
{
	t_ui_state	*state;

	state = get_ui_state(NULL);
	state->ui_text_color = color;
}

SDL_Color	get_text_color(void)
{
	t_ui_state	*state;

	state = get_ui_state(NULL);
	return (get_sdl_color(state->ui_text_color));
}

t_ivec2	render_text_internal(char *str, t_window *window,
						TTF_Font *font, t_ivec2 pos)
{
	SDL_Rect		text_rect;
	t_ivec2			size;
	SDL_Surface		*surface_message;
	SDL_Texture		*message;

	surface_message
		= TTF_RenderText_Blended(font, str, get_text_color());
	message
		= SDL_CreateTextureFromSurface(window->sdl_renderer, surface_message);
	text_rect.w = 0;
	text_rect.h = 0;
	TTF_SizeText(font, str, &text_rect.w, &text_rect.h);
	text_rect.x = pos.x;
	text_rect.y = pos.y;
	SDL_SetRenderTarget(window->sdl_renderer, window->text_texture);
	SDL_RenderCopy(window->sdl_renderer, message, NULL, &text_rect);
	SDL_SetRenderTarget(window->sdl_renderer, NULL);
	SDL_FreeSurface(surface_message);
	SDL_DestroyTexture(message);
	size.x = text_rect.w;
	size.y = text_rect.h;
	return (size);
}

t_ivec2	render_text(char *str, int x, int y)
{
	t_ivec2		pos;
	t_window	*window;
	t_ui_state	*state;
	TTF_Font	*font;

	window = get_window(NULL);
	state = get_ui_state(NULL);
	font = state->current_font;
	pos.x = x;
	pos.y = y;
	return (render_text_internal(str, window, font, pos));
}
