/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	tick_forward(t_level *level, t_game_state *game_state)
{
	if (*game_state == GAME_STATE_MAIN_MENU)
		camera_path_set(&level->main_menu_anim, &level->cam);
	else
	{
		if (*game_state != GAME_STATE_EDITOR)
			game_logic(level, game_state);
		player_movement(level);
	}
	update_camera(level);
	door_animate(level);
	if (*game_state == GAME_STATE_INGAME)
	{
		projectiles_update(level);
		enemies_update(level);
	}
}

void	viewmodel(t_window *window, t_level *level,
								t_game_state *game_state)
{
	int	normal_map_disabled;

	if (*game_state != GAME_STATE_INGAME)
		return ;
	if (level->game_logic.reload_start_time)
		play_animation(&level->game_models.viewmodel,
			&level->game_models.reload_animation,
			level->game_logic.reload_start_time);
	level->visible.tri_amount = 0;
	merge_viewmodel(level);
	screen_space_partition(level);
	level->render_is_first_pass = TRUE;
	normal_map_disabled = level->ui.normal_map_disabled;
	level->ui.normal_map_disabled = TRUE;
	render_raycast(window, level, game_state);
	level->ui.normal_map_disabled = normal_map_disabled;
	level->render_is_first_pass = FALSE;
}

void	dnukem(t_window *window, t_level *level, t_game_state game_state)
{
	unsigned int	ssp_time;
	unsigned int	cull_time;
	unsigned int	render_time;
	unsigned int	frame_time;

	while (1)
	{
		frame_time = SDL_GetTicks();
		read_input(window, level, &game_state);
		tick_forward(level, &game_state);
		cull_time = SDL_GetTicks();
		viewmodel(window, level, &game_state);
		culling(level);
		merge_game_models(level, game_state);
		level->ui.cull_time = SDL_GetTicks() - cull_time;
		ssp_time = SDL_GetTicks();
		screen_space_partition(level);
		level->ui.ssp_time = SDL_GetTicks() - ssp_time;
		render_time = SDL_GetTicks();
		render(window, level, &game_state);
		level->ui.render_time = SDL_GetTicks() - render_time;
		level->ui.frame_time = SDL_GetTicks() - frame_time;
	}
}

int	main(int argc, char **argv)
{
	t_window		*window;
	t_level			*level;
	t_game_state	game_state;

	(void)argc;
	(void)argv;
	game_state = GAME_STATE_MAIN_MENU;
	level = (t_level *)malloc(sizeof(t_level));
	if (!level)
		ft_error("memory allocation failed\n");
	ft_bzero(level, sizeof(t_level));
	init_window(&window);
	init_ui(window, level);
	init_audio(level);
	init_embedded(level);
	Mix_PlayMusic(level->audio.title_music, -1);
	init_screen_space_partition(level);
	init_culling(level);
	while (!level->level_initialized)
	{
		read_input(window, level, &game_state);
		render(window, level, &game_state);
	}
	dnukem(window, level, game_state);
	return (0);
}
