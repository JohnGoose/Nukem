/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_path_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	camera_path_delete_pos(t_camera_path *path, int index)
{
	while (index < path->amount - 1)
	{
		path->pos[index] = path->pos[index + 1];
		index++;
	}
	path->pos = (t_player_pos *)ft_realloc(path->pos,
			sizeof(t_player_pos) * path->amount,
			sizeof(t_player_pos) * (path->amount - 1));
	path->amount--;
}

void	camera_path_add_pos(t_camera_path *path, t_camera c)
{
	path->pos = (t_player_pos *)ft_realloc(path->pos,
			sizeof(t_player_pos) * path->amount,
			sizeof(t_player_pos) * (path->amount + 1));
	path->pos[path->amount].look_side = c.look_side;
	path->pos[path->amount].look_up = c.look_up;
	path->pos[path->amount].pos = c.pos;
	path->amount++;
}

float	path_get_percentage(t_camera_path *path)
{
	float	time;

	time = (SDL_GetTicks() - path->start_time)
		/ (1000.0 * path->duration);
	if (time > 2)
	{
		path->start_time = SDL_GetTicks();
		time = 0;
	}
	else if (time > 1)
	{
		if (path->loop)
		{
			path->start_time = SDL_GetTicks();
			time = 0;
		}
		else
			time = 1 - (time - 1);
	}
	return (time);
}

void	camera_path_set(t_camera_path *path, t_camera *cam)
{
	float	time;

	memset(cam, 0, sizeof(t_camera));
	if (path->amount == 0)
		return ;
	if (path->amount == 1)
	{
		cam->look_side = path->pos[0].look_side;
		cam->look_up = path->pos[0].look_up;
		cam->pos = path->pos[0].pos;
		return ;
	}
	time = path_get_percentage(path);
	if (path->amount == 2)
		cam_lerp(cam, path->pos[0], path->pos[1], time);
	else
		camera_path_bezier(path, cam, time);
}
