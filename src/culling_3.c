/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   culling_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/05 21:38:36 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	opacity_culling(t_level *level, int i, t_obj *obj)
{
	int		amount;
	int		k;

	level->all.tris[i].opacity_obj_all.tri_amount = 0;
	amount = 0;
	k = 0;
	while (k < obj->tri_amount)
	{
		if ((obj->tris[k].isgrid
				|| cull_ahead(level->all.tris[i].normal,
					level->all.tris[i].verts[0].pos, obj->tris[k]))
			&& backface_culling(level->cam.pos, obj->tris[k]))
		{
			level->all.tris[i].opacity_obj_all.tris[amount]
				= obj->tris[k];
			amount++;
		}
		k++;
	}
	level->all.tris[i].opacity_obj_all.tri_amount = amount;
}

void	free_culling(t_level *level)
{
	int	i;

	i = 0;
	while (i < level->all.tri_amount)
	{
		free(level->all.tris[i].reflection_obj_all.tris);
		free(level->all.tris[i].reflection_obj_first_bounce.tris);
		free(level->all.tris[i].opacity_obj_all.tris);
		free(level->all.tris[i].shadow_faces.tris);
		i++;
	}
}

void	init_culling(t_level *level)
{
	int	i;

	i = -1;
	while (++i < level->all.tri_amount)
	{
		level->all.tris[i].texture = &level->texture;
		level->all.tris[i].reflection_obj_all.tris
			= (t_tri *)malloc(sizeof(t_tri) * level->all.tri_amount);
		level->all.tris[i].reflection_obj_first_bounce.tris
			= (t_tri *)malloc(sizeof(t_tri) * level->all.tri_amount);
		level->all.tris[i].opacity_obj_all.tris
			= (t_tri *)malloc(sizeof(t_tri) * level->all.tri_amount);
		level->all.tris[i].shadow_faces.tris
			= (t_tri *)malloc(sizeof(t_tri) * level->all.tri_amount);
		if (!level->all.tris[i].reflection_obj_first_bounce.tris
			|| !level->all.tris[i].opacity_obj_all.tris
			|| !level->all.tris[i].reflection_obj_all.tris
			|| !level->all.tris[i].shadow_faces.tris)
			ft_error("culling malloc fail");
		level->all.tris[i].dynamic = FALSE;
	}
	static_culling(level);
}

void	calculate_corner_vectors(t_vec3 corner[4], t_camera *cam)
{
	float	ym;
	float	xm;

	ym = -cam->fov_y / 2;
	xm = -cam->fov_x / 2;
	corner[0].x = cam->front.x + cam->up.x * ym + cam->side.x * xm;
	corner[0].y = cam->front.y + cam->up.y * ym + cam->side.y * xm;
	corner[0].z = cam->front.z + cam->up.z * ym + cam->side.z * xm;
	ym = -cam->fov_y / 2;
	xm = cam->fov_x - cam->fov_x / 2;
	corner[1].x = cam->front.x + cam->up.x * ym + cam->side.x * xm;
	corner[1].y = cam->front.y + cam->up.y * ym + cam->side.y * xm;
	corner[1].z = cam->front.z + cam->up.z * ym + cam->side.z * xm;
	ym = cam->fov_y - cam->fov_y / 2;
	xm = -cam->fov_x / 2;
	corner[2].x = cam->front.x + cam->up.x * ym + cam->side.x * xm;
	corner[2].y = cam->front.y + cam->up.y * ym + cam->side.y * xm;
	corner[2].z = cam->front.z + cam->up.z * ym + cam->side.z * xm;
	ym = cam->fov_y - cam->fov_y / 2;
	xm = cam->fov_x - cam->fov_x / 2;
	corner[3].x = cam->front.x + cam->up.x * ym + cam->side.x * xm;
	corner[3].y = cam->front.y + cam->up.y * ym + cam->side.y * xm;
	corner[3].z = cam->front.z + cam->up.z * ym + cam->side.z * xm;
}

void	skybox_culling(t_level *level, t_camera *cam,
										t_vec3 side_normals[4])
{
	int	visible_amount;
	int	i;

	visible_amount = 0;
	i = 0;
	while (i < level->sky.all.tri_amount)
	{
		if (cull_behind(cam->front, (t_vec3){0, 0, 0},
			level->sky.all.tris[i])
			&& frustrum_culling(side_normals,
				(t_vec3){0, 0, 0}, level->sky.all.tris[i]))
		{
			level->sky.visible.tris[visible_amount] = level->sky.all.tris[i];
			visible_amount++;
		}
		i++;
	}
	level->sky.visible.tri_amount = visible_amount;
}
