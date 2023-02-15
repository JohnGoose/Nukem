/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spray_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	cast_uv(t_tri t, t_ray ray, t_vec2 *uv)
{
	t_vec3	vec;
	t_vec3	tvec;
	float	u;
	float	v;
	float	tmp;

	vec_cross(&vec, ray.dir, t.v0v2);
	tmp = 1 / vec_dot(vec, t.v0v1);
	vec_sub(&tvec, ray.pos, t.verts[0].pos);
	u = vec_dot(tvec, vec) * tmp;
	vec_cross(&vec, tvec, t.v0v1);
	v = vec_dot(ray.dir, vec) * tmp;
	tmp = vec_dot(vec, t.v0v2) * tmp;
	if (tmp > SPRAY_MAX_DIST || tmp < 0)
		return (0);
	uv->x = u;
	uv->y = v;
	return (1);
}

int	raycast_face_pos(t_ray *r, t_obj *object)
{
	float	dist;
	float	tmp;
	int		i;
	int		new_hit;

	i = 0;
	dist = FLT_MAX;
	new_hit = -1;
	while (i < object->tri_amount)
	{
		tmp = cast_face(object->tris[i], *r, NULL);
		if (tmp > 0 && tmp < dist)
		{
			dist = tmp;
			new_hit = object->tris[i].index;
		}
		i++;
	}
	if (new_hit == -1)
		return (-1);
	vec_mult(&r->dir, dist);
	return (new_hit);
}

t_vec2	corner_cam_diff(int corner, t_camera *cam)
{
	int		diff;
	t_vec2	cam_diff;

	diff = fmin(RES_Y, RES_X) * (SPRAY_FROM_VIEW_SIZE / 2);
	if (corner == 0)
	{
		cam_diff.y = cam->fov_y / RES_Y * (RES_Y / 2 - diff) - cam->fov_y / 2;
		cam_diff.x = cam->fov_x / RES_X * (RES_X / 2 - diff) - cam->fov_x / 2;
	}
	else if (corner == 1)
	{
		cam_diff.y = cam->fov_y / RES_Y * (RES_Y / 2 + diff) - cam->fov_y / 2;
		cam_diff.x = cam->fov_x / RES_X * (RES_X / 2 - diff) - cam->fov_x / 2;
	}
	else if (corner == 2)
	{
		cam_diff.y = cam->fov_y / RES_Y * (RES_Y / 2 - diff) - cam->fov_y / 2;
		cam_diff.x = cam->fov_x / RES_X * (RES_X / 2 + diff) - cam->fov_x / 2;
	}
	else
	{
		cam_diff.y = cam->fov_y / RES_Y * (RES_Y / 2 + diff) - cam->fov_y / 2;
		cam_diff.x = cam->fov_x / RES_X * (RES_X / 2 + diff) - cam->fov_x / 2;
	}
	return (cam_diff);
}

void	move_cam(t_level *l, t_camera *cam, int hit, t_ray r)
{
	t_vec3	tmp;

	vec_add(&cam->pos, r.pos, r.dir);
	tmp = l->all.tris[hit].normal;
	vec_mult(&tmp, l->ui.spray_size);
	vec_add(&cam->pos, cam->pos, tmp);
	cam->front = l->all.tris[hit].normal;
	vec_mult(&cam->front, -1);
	tmp.x = 0;
	tmp.y = -1;
	tmp.z = 0;
	vec_cross(&cam->side, cam->front, tmp);
	vec_normalize(&cam->side);
	vec_cross(&cam->up, cam->front, cam->side);
	vec_normalize(&cam->up);
}

void	rot_to_corner(t_ray *r, t_camera cam, t_vec2 cam_diff)
{
	r->dir = cam.front;
	r->pos = cam.pos;
	r->dir.x += cam.up.x * cam_diff.y + cam.side.x * cam_diff.x;
	r->dir.y += cam.up.y * cam_diff.y + cam.side.y * cam_diff.x;
	r->dir.z += cam.up.z * cam_diff.y + cam.side.z * cam_diff.x;
}
