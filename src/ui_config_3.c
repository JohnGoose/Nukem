/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	nothing_selected(t_level *level)
{
	int	i;

	i = 0;
	while (i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
			return (0);
		i++;
	}
	return (1);
}

void	ui_render_info(t_editor_ui *ui, t_level *level)
{
	char		buf[100];

	ft_sprintf(buf, "fps: %d", get_fps());
	text(buf);
	ft_sprintf(buf, " |   cull: %ums", ui->cull_time);
	text(buf);
	ft_sprintf(buf, " |   ssp: %ums", ui->ssp_time);
	text(buf);
	ft_sprintf(buf, " |   |   raycast amount: %uk", ui->total_raycasts / 1000);
	text(buf);
	ft_sprintf(buf, " |   |   raycast: %ums", ui->raycast_time);
	text(buf);
	ft_sprintf(buf, " |   |   post:       %ums", ui->post_time);
	text(buf);
	ft_sprintf(buf, " |   |   raster:   %ums", ui->raster_time);
	text(buf);
	ft_sprintf(buf, " |   |   ui:           %ums", ui->ui_time);
	text(buf);
	ft_sprintf(buf, " |   render:      %ums", ui->render_time);
	text(buf);
	ft_sprintf(buf, "frame: %ums", ui->frame_time);
	text(buf);
	ft_sprintf(buf, "faces: %d / %d",
		level->all.tri_amount, level->visible.tri_amount);
	text(buf);
}

void	center_screen_print_line(t_vec2 dir, unsigned int color)
{
	t_window	*window;
	t_vec3		start;
	t_vec3		stop;

	window = get_window(NULL);
	start.x = RES_X / 2;
	start.y = RES_Y / 2;
	start.z = 0;
	stop.x = RES_X / 2 + (dir.x * 20);
	stop.y = RES_Y / 2 + (dir.y * -20);
	stop.z = 0;
	print_line(start, stop, color, window->ui_texture_pixels);
}

void	ui_physics_info(t_editor_ui *ui, t_level *level)
{
	char	buf[100];
	t_vec2	tmp;
	t_vec3	n;

	button(&ui->physics_debug, "physics debug");
	if (!ui->physics_debug)
		return ;
	tmp.x = level->game_logic.player.vel.x;
	tmp.y = level->game_logic.player.vel.z;
	center_screen_print_line(tmp, 0xff0000ff);
	ft_sprintf(buf, "xz vel: %.2fm/s", level->ui.horizontal_velocity);
	set_text_color(0xff0000ff);
	render_text(buf, RES_X / 2, RES_Y / 2 + (UI_ELEMENT_HEIGHT * 1));
	center_screen_print_line(ui->wishdir, 0xff00ff);
	set_text_color(0xff00ff);
	render_text("wishdir", RES_X / 2, RES_Y / 2 + (UI_ELEMENT_HEIGHT * 2));
	n = level->cam.front;
	vec_normalize(&n);
	tmp.x = n.x;
	tmp.y = n.z;
	center_screen_print_line(tmp, 0xffff);
	set_text_color(0xffff);
	render_text("camera", RES_X / 2, RES_Y / 2 + (UI_ELEMENT_HEIGHT * 3));
}

void	ui_post_process_go_back(t_level *level)
{
	if (call("back", NULL))
	{
		if (level->ui.state.ui_location
			== UI_LOCATION_POST_PROCESS_SETTINGS)
			level->ui.state.ui_location = UI_LOCATION_MAIN;
		else
			level->ui.main_menu = MAIN_MENU_LOCATION_SETTINGS;
	}
}
