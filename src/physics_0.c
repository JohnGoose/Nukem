/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/09 20:05:24 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	input_player_movement(t_vec3 *wishdir, const Uint8 *keys)
{
	if (keys[SDL_SCANCODE_W])
		wishdir->z += 1;
	if (keys[SDL_SCANCODE_S])
		wishdir->z -= 1;
	if (keys[SDL_SCANCODE_A])
		wishdir->x -= 1;
	if (keys[SDL_SCANCODE_D])
		wishdir->x += 1;
}

void	uv_linear_zoom(t_level *level, const Uint8 *keys)
{
	if (keys[SDL_SCANCODE_MINUS] && level->ui.state.uv_zoom > 0.5)
	{
		level->ui.state.uv_zoom /= 1.01;
		level->ui.state.uv_pos.x /= 1.01;
		level->ui.state.uv_pos.y /= 1.01;
	}
	if (keys[SDL_SCANCODE_EQUALS] && level->ui.state.uv_zoom < 10)
	{
		level->ui.state.uv_zoom *= 1.01;
		level->ui.state.uv_pos.x *= 1.01;
		level->ui.state.uv_pos.y *= 1.01;
	}
}

void	input_uv(t_level *level, const Uint8 *keys)
{
	if (level->ui.state.ui_location == UI_LOCATION_UV_EDITOR)
	{
		if (keys[SDL_SCANCODE_LEFT])
			level->ui.state.uv_pos.x -= 10;
		if (keys[SDL_SCANCODE_RIGHT])
			level->ui.state.uv_pos.x += 10;
		if (keys[SDL_SCANCODE_UP])
			level->ui.state.uv_pos.y -= 10;
		if (keys[SDL_SCANCODE_DOWN])
			level->ui.state.uv_pos.y += 10;
		uv_linear_zoom(level, keys);
	}
	else
	{
		if (keys[SDL_SCANCODE_LEFT])
			level->cam.look_side -= 0.04;
		if (keys[SDL_SCANCODE_RIGHT])
			level->cam.look_side += 0.04;
	}
}

void	player_input(t_level *level, t_player *player)
{
	const Uint8	*keys;

	keys = SDL_GetKeyboardState(NULL);
	ft_bzero(&player->wishdir, sizeof(t_vec3));
	if (level->ui.state.text_input_enable
		|| level->game_logic.death_start_time)
		return ;
	input_player_movement(&player->wishdir, keys);
	input_uv(level, keys);
	if (keys[SDL_SCANCODE_SPACE])
		player->wishdir.y -= 1;
	if (keys[SDL_SCANCODE_LSHIFT] && level->ui.noclip)
		player->wishdir.y += 1;
	if (keys[SDL_SCANCODE_LCTRL] && !level->ui.noclip)
	{
		player->height = CROUCHED_HEIGHT;
		player->move_speed = CROUCH_SPEED;
	}
	else if (keys[SDL_SCANCODE_LSHIFT] && !level->ui.noclip)
		player->move_speed = WALK_SPEED;
	else if (!level->ui.noclip)
		player->move_speed = RUN_SPEED;
}

int	player_collision(t_vec3 *vel, t_vec3 *pos, t_level *level,
															float height)
{
	t_ray	r;
	float	dist;
	int		index;
	t_vec3	normal;

	dist = 0;
	r.pos = *pos;
	r.dir = (t_vec3){0, 1, 0};
	dist = cast_all(r, level, &index);
	if (dist != FLT_MAX)
	{
		normal = level->all.tris[index].normal;
		if (normal.y < 0 && dist <= height)
			pos->y -= height - dist;
	}
	r.pos = *pos;
	r.dir = *vel;
	physics_raycast(r, level, vel);
	r.pos.y += height / PLAYER_HEIGHT_MAGIC;
	r.dir = *vel;
	physics_raycast(r, level, vel);
	return (0);
}
