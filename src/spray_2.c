/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spray_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	face_in_front(t_camera *cam, t_level *level)
{
	t_vec2	cam_diff;
	t_ray	r;
	int		hit;

	if (!level->ui.spray_from_view)
	{
		cam->fov_y = M_PI / 2;
		cam->fov_x = M_PI / 2 * ((float)RES_X / RES_Y);
	}
	cam_diff.y = cam->fov_y / RES_Y * (RES_Y / 2) - cam->fov_y / 2;
	cam_diff.x = cam->fov_x / RES_X * (RES_X / 2) - cam->fov_x / 2;
	rot_to_corner(&r, *cam, cam_diff);
	hit = raycast_face_pos(&r, &level->all);
	if (hit != -1 && !level->ui.spray_from_view)
		move_cam(level, cam, hit, r);
	return (hit);
}

void	spray(t_camera cam, t_level *level)
{
	t_vec2	uv;
	t_ray	r;
	t_vec2	corner[4];
	int		hit;
	int		i;

	hit = face_in_front(&cam, level);
	if (hit == -1)
		return ;
	i = -1;
	while (++i < 4)
	{
		rot_to_corner(&r, cam, corner_cam_diff(i, &cam));
		if (!cast_uv(level->all.tris[hit], r, &uv))
			return ;
		uv = uv_to_2d(level->all.tris[hit], uv, 0);
		corner[i].x = uv.x;
		corner[i].y = 1.0 - uv.y;
		corner[i].x *= level->texture.width;
		corner[i].y *= level->texture.height;
	}
	vec2_sub(&corner[1], corner[0], corner[1]);
	vec2_sub(&corner[3], corner[2], corner[3]);
	draw_square(level, corner, &level->all.tris[hit]);
}
