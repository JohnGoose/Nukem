# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/08 13:46:11 by vkuikka           #+#    #+#              #
#    Updated: 2022/04/15 15:07:42 by rpehkone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = doom-nukem
SRCDIR = src/
SRC = 2d_intersect_0.c \
	2d_intersect_1.c \
	animation_0.c \
	animation_1.c \
	audio_0.c \
	bake_0.c \
	bake_1.c \
	bake_2.c \
	bloom_0.c \
	bloom_1.c \
	bmp_read_0.c \
	box_blur_0.c \
	camera_path_0.c \
	camera_path_1.c \
	cast_face_0.c \
	color_0.c \
	color_1.c \
	color_2.c \
	color_3.c \
	color_4.c \
	color_hsl_0.c \
	culling_0.c \
	culling_1.c \
	culling_2.c \
	culling_3.c \
	culling_4.c \
	culling_occlusion_0.c \
	culling_occlusion_1.c \
	deserialize_0.c \
	deserialize_1.c \
	deserialize_2.c \
	deserialize_3.c \
	deserialize_4.c \
	deserialize_5.c \
	deserialize_primitive_0.c \
	distortion_0.c \
	door_editor_0.c \
	door_editor_1.c \
	door_editor_2.c \
	door_runtime_0.c \
	dynamic_geometry_0.c \
	dynamic_geometry_1.c \
	editor_set_state_0.c \
	editor_set_state_1.c \
	embed_macos.c \
	enemies_0.c \
	enemy_state_machine_0.c \
	file_read_0.c \
	filesystem_macos_0.c \
	find_quads_0.c \
	game_logic_0.c \
	game_logic_1.c \
	game_logic_editor_0.c \
	game_logic_editor_1.c \
	game_logic_editor_2.c \
	gizmo_0.c \
	gizmo_1.c \
	gizmo_2.c \
	hud_0.c \
	hud_1.c \
	init_0.c \
	init_1.c \
	init_2.c \
	init_3.c \
	input_0.c \
	input_1.c \
	light_editor_0.c \
	light_editor_1.c \
	main_0.c \
	main_1.c \
	main_menu_0.c \
	main_menu_1.c \
	nonfatal_error_0.c \
	obj_editor_0.c \
	obj_editor_1.c \
	obj_read_0.c \
	obj_read_1.c \
	obj_read_2.c \
	physics_0.c \
	physics_1.c \
	physics_2.c \
	physics_3.c \
	projectile_0.c \
	radial_gradient_0.c \
	raycast_0.c \
	raycast_1.c \
	raycast_2.c \
	read_write_0.c \
	render_raycast_0.c \
	screen_space_partition_0.c \
	screen_space_partition_1.c \
	screen_space_partition_2.c \
	select_0.c \
	select_1.c \
	serialize_0.c \
	serialize_1.c \
	serialize_2.c \
	serialize_3.c \
	serialize_4.c \
	serialize_primitive_0.c \
	shader_perlin_0.c \
	shader_perlin_1.c \
	shader_perlin_2.c \
	shaders_0.c \
	shaders_1.c \
	spray_0.c \
	spray_1.c \
	spray_2.c \
	sprite_0.c \
	ssao_0.c \
	ssao_1.c \
	ui_0.c \
	ui_1.c \
	ui_2.c \
	ui_config_0.c \
	ui_config_1.c \
	ui_config_2.c \
	ui_config_3.c \
	ui_config_4.c \
	ui_config_5.c \
	ui_config_6.c \
	ui_config_7.c \
	ui_config_8.c \
	ui_elements_0.c \
	ui_elements_1.c \
	ui_elements_2.c \
	ui_elements_3.c \
	ui_elements_4.c \
	uv_editor_0.c \
	uv_editor_1.c \
	uv_editor_2.c \
	uv_overlap_0.c \
	uv_overlap_1.c \
	uv_overlap_2.c \
	vectors_0.c \
	vectors_1.c \
	vectors_2.c \
	vectors_3.c \
	vectors_4.c \
	wireframe_0.c \
	wireframe_1.c \
	wireframe_2.c

AUDIO_EFFECT = death.ogg doorsliding.wav gunshot.wav jump.wav osrsMonsterDeath.wav teleport.wav windowShatter.wav door.wav gettinghit.wav jetpack.wav osrsDeath.wav reload.wav victory.wav
MUSIC = ingame.ogg main_menu.ogg
AUDIO_FILES=$(addprefix music/, $(MUSIC))
AUDIO_FILES+=$(addprefix effects/, $(AUDIO_EFFECT))
AUDIO=$(addprefix audio/, $(AUDIO_FILES))

EMBED_FILES = title.bmp skybox.obj digital.ttf roboto.ttf ammo_diff.bmp health_diff.bmp pickup_box.obj enemy_diff.bmp lightsprite.bmp skybox.bmp spray.bmp projectile.bmp enemy_shoot.obj
EMBED_LEVEL_FILES = ship_final.obj out.bmp normal.bmp
EMBED_LEVEL=$(addprefix level/, $(EMBED_LEVEL_FILES))
EMBED_VIEWMODEL_FILES = viewmodel_000000.obj viewmodel_000001.obj viewmodel_000002.obj viewmodel_000003.obj viewmodel_000004.obj viewmodel_000005.obj viewmodel_000006.obj viewmodel_000007.obj viewmodel_000008.obj viewmodel_000009.obj viewmodel_000010.obj vulcan_diff.bmp
EMBED_VIEWMODEL=$(addprefix viewmodel/, $(EMBED_VIEWMODEL_FILES))
EMBED_ANIM_RUN_FILES = enemy_run_000000.obj enemy_run_000002.obj enemy_run_000004.obj enemy_run_000006.obj enemy_run_000008.obj enemy_run_000001.obj enemy_run_000003.obj enemy_run_000005.obj enemy_run_000007.obj enemy_run_000009.obj
EMBED_ANIM_DIE_FILES = enemy_die_000000.obj enemy_die_000002.obj enemy_die_000004.obj enemy_die_000006.obj enemy_die_000008.obj enemy_die_000010.obj enemy_die_000012.obj enemy_die_000001.obj enemy_die_000003.obj enemy_die_000005.obj enemy_die_000007.obj enemy_die_000009.obj enemy_die_000011.obj enemy_die_000013.obj
EMBED_ANIM_RUN=$(addprefix enemy_run/, $(EMBED_ANIM_RUN_FILES))
EMBED_ANIM_DIE=$(addprefix enemy_die/, $(EMBED_ANIM_DIE_FILES))
EMBED_FILES += $(EMBED_ANIM_RUN)
EMBED_FILES += $(EMBED_ANIM_DIE)
EMBED_FILES += $(EMBED_VIEWMODEL)
EMBED_FILES2 = $(EMBED_LEVEL)
EMBED_FILES2 += $(VIEWMODEL)
EMBED_FILES2 += $(AUDIO)
EMBED=$(addprefix embed/, $(EMBED_FILES))
EMBED2=$(addprefix embed/, $(EMBED_FILES2))

LIB = libft/libft.a libft/ft_printf/libftprintf.a
INCLUDE = -I libft/includes -I ./ -I src/ -I libft/ft_printf/
SDL_FRAMEWORKS = -framework SDL2 -framework SDL2_ttf -framework SDL2_mixer
SDL_HEADER = -I SDL2.framework/Headers
FLAGS = -Wall -Wextra -Werror -O3

.PHONY: clean fclean re all
FILES=$(addprefix src/, $(SRC))

all: $(NAME)

$(NAME):
	@make -C libft
	@touch embed.c
	@touch embed2.c
	@gcc -o embed.o -c embed.c
	@gcc -o embed2.o -c embed2.c
	@$(foreach file, $(EMBED), END=$$(echo $(file) | sed 's:.*/::' | tail -c 16); echo "$$END"; ld -r -o embed.o -sectcreate embed "$$END" $(file) embed.o;)
	@$(foreach file, $(EMBED2), END=$$(echo $(file) | sed 's:.*/::' | tail -c 16); echo "$$END"; ld -r -o embed2.o -sectcreate embed "$$END" $(file) embed2.o;)
	-gcc $(FLAGS) $(SDL_HEADER) $(SDL_FRAMEWORKS) -F ./ $(FILES) embed.o embed2.o $(LIB) $(INCLUDE) -o $(NAME) -rpath @executable_path
	@rm embed.o embed.c embed2.o embed2.c

clean:
	@make clean -C libft

fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft

re: fclean
	@make all
