/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/06 00:00:59 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ui_config_selected_faces(t_level *level)
{
	int	selected_amount;
	int	selected_index;
	int	i;

	set_text_color(WF_SELECTED_COL);
	selected_index = 0;
	selected_amount = get_selected_amount(level);
	i = -1;
	while (++i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
		{
			if (!selected_index)
			{
				ui_confing_face_settings(level,
					selected_amount, &level->all.tris[i]);
				selected_index = i + 1;
				continue ;
			}
			copy_tri_settings(&level->all.tris[i],
				&level->all.tris[selected_index - 1]);
		}
	}
}

void	make_fileopen_call(t_level *level, char *file)
{
	char	absolute_filename[PATH_MAX + NAME_MAX];

	ft_strcpy(absolute_filename, level->ui.state.directory);
	go_in_dir(absolute_filename, file);
	level->ui.state.open_file(level, absolute_filename);
}

void	ui_loop_directory_callback(int isdir, char *name, void *data)
{
	t_level	*level;
	char	*exten;
	int		extension_match;

	level = (t_level *)data;
	if (!isdir && !level->ui.state.find_dir)
	{
		exten = level->ui.state.extension;
		extension_match = FALSE;
		if (ft_strlen(name) > ft_strlen(exten)
			&& !ft_strcmp(exten, &name[ft_strlen(name) - ft_strlen(exten)]))
			extension_match = TRUE;
		if (level->ui.state.find_extension != extension_match)
		{
			if (level->ui.state.ui_location == UI_LOCATION_FILE_OPEN)
			{
				if (call(name, NULL))
					make_fileopen_call(level, name);
			}
			else
				text(name);
		}
	}
	else if (isdir && level->ui.state.find_dir && call(name, NULL))
		go_in_dir(level->ui.state.directory, name);
}

void	ui_render_directory_header(t_level *level)
{
	char		buf[100];

	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	text(level->ui.state.directory);
	if (level->ui.state.ui_location == UI_LOCATION_FILE_OPEN)
	{
		ft_sprintf(buf, "select  %s  file", level->ui.state.extension);
		text(buf);
	}
	else
		text("Save file");
	if (call("close", NULL))
	{
		if (level->ui.state.open_file == &open_level
			|| level->ui.state.open_file == &set_spray)
			level->ui.main_menu = MAIN_MENU_LOCATION_MAIN;
		level->ui.state.ui_location = UI_LOCATION_MAIN;
	}
	if (call("up dir ..", NULL))
		path_up_dir(level->ui.state.directory);
}

void	ui_render_directory(t_level *level)
{
	ui_render_directory_header(level);
	set_text_color(UI_DIRECTORY_FOLDER_COLOR);
	level->ui.state.find_dir = TRUE;
	level->ui.state.find_extension = FALSE;
	loop_directory(level->ui.state.directory, (void *)level,
		&ui_loop_directory_callback);
	set_text_color(UI_DIRECTORY_FILE_WANTED_COLOR);
	level->ui.state.find_dir = FALSE;
	loop_directory(level->ui.state.directory, (void *)level,
		&ui_loop_directory_callback);
	set_text_color(UI_DIRECTORY_FILE_OTHER_COLOR);
	level->ui.state.find_extension = TRUE;
	loop_directory(level->ui.state.directory, (void *)level,
		&ui_loop_directory_callback);
	set_text_color(UI_LEVEL_SETTINGS_TEXT_COLOR);
	if (level->ui.state.ui_location == UI_LOCATION_FILE_SAVE)
	{
		text("save as:");
		text_input(level->ui.state.save_filename, level);
		if (call("save", NULL))
		{
			save_level(level);
			level->ui.state.ui_location = UI_LOCATION_MAIN;
		}
	}
}
