/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ssao_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ssao_color(t_window *win, t_level *level, float amount, t_ivec2 coords)
{
	unsigned int	pixel;

	pixel = coords.x + coords.y * RES_X;
	if (level->ui.ssao_debug)
		win->frame_buffer[pixel] = crossfade(0xffffff, 0, amount * 0xff, 0xff);
	else
		win->frame_buffer[pixel] = crossfade(win->frame_buffer[pixel] >> 8, 0,
				amount * 0xff, 0xff);
}

void	ssao_calculate(t_window *win, t_level *level, int thread_id)
{
	t_ivec2	i;
	t_ssao	ssao;
	float	darkness;

	ssao.radius = level->ui.ssao_radius;
	ssao.radius -= ssao.radius % level->ui.raycast_quality;
	i.x = thread_id * level->ui.raycast_quality;
	while (i.x < RES_X)
	{
		i.y = 0;
		while (i.y < RES_Y)
		{
			if (win->depth_buffer[i.x + i.y * RES_X] != FLT_MAX)
			{
				ssao.kernel_center = i;
				darkness = surrounding_diff(ssao, level, win);
				ssao_color(win, level, darkness, i);
			}
			i.y += level->ui.raycast_quality;
		}
		i.x += THREAD_AMOUNT * level->ui.raycast_quality;
	}
}

int	ssao_thread(void *data_pointer)
{
	t_rthread	*thread;

	thread = data_pointer;
	ssao_calculate(thread->window, thread->level, thread->id);
	return (0);
}

void	ssao(t_window *window, t_level *level)
{
	SDL_Thread		*threads[THREAD_AMOUNT];
	t_rthread		thread_data[THREAD_AMOUNT];
	int				i;

	i = -1;
	while (++i < THREAD_AMOUNT)
	{
		thread_data[i].id = i;
		thread_data[i].level = level;
		thread_data[i].window = window;
		threads[i] = SDL_CreateThread(ssao_thread, "ssao",
				(void *)&thread_data[i]);
	}
	i = -1;
	while (++i < THREAD_AMOUNT)
		SDL_WaitThread(threads[i], NULL);
}
