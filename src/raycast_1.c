/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/04/05 21:44:04 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	cast_all_color(t_level *l, t_obj *obj, t_cast_result *res,
							int apply_fog)
{
	int		new_hit;

	vec_normalize(&res->ray.dir);
	new_hit = cast_loop(obj, res);
	if (new_hit == -1 && l->render_is_first_pass)
	{
		res->color = 0;
		return ;
	}
	if (new_hit == -1)
		skybox(l, res);
	else
	{
		res->dynamic = obj->tris[new_hit].dynamic;
		res->face_index = obj->tris[new_hit].index;
		res->texture = obj->tris[new_hit].texture;
		face_color(res->uv.x, res->uv.y, obj->tris[new_hit], res);
		vec_mult(&res->ray.dir, res->dist);
		vec_add(&res->ray.pos, res->ray.pos, res->ray.dir);
		raytrace(res, obj, l);
	}
	if (l->ui.fog && apply_fog)
		fog(&res->color, res->dist, l->ui.fog_color.color, l);
}

t_ray	ray_set(t_camera *cam, t_ivec2 xy)
{
	t_ray	res;
	float	xm;
	float	ym;

	xm = cam->fov_y * ((float)RES_X / RES_Y);
	ym = cam->fov_y;
	xm = xm / RES_X * xy.x - xm / 2;
	ym = ym / RES_Y * xy.y - ym / 2;
	res.dir.x = cam->front.x + cam->up.x * ym + cam->side.x * xm;
	res.dir.y = cam->front.y + cam->up.y * ym + cam->side.y * xm;
	res.dir.z = cam->front.z + cam->up.z * ym + cam->side.z * xm;
	res.pos = cam->pos;
	return (res);
}

void	cast_result_set(t_cast_result *res, t_level *level)
{
	res->raytracing = level->ui.raytracing;
	if (!level->ui.normal_map_disabled)
		res->normal_map = &level->normal_map;
	else
		res->normal_map = NULL;
	res->spray_overlay = level->spray_overlay;
	if (level->bake_status != BAKE_NOT_BAKED)
		res->baked = level->baked;
	else
		res->baked = NULL;
	res->reflection_depth = 0;
	res->face_index = -1;
	res->light.r = 0;
	res->light.g = 0;
	res->light.b = 0;
}

void	set_render_result(t_window *window,
								t_cast_result res, unsigned int i)
{
	window->frame_buffer[i] = res.color;
	window->depth_buffer[i] = res.dist;
	window->brightness_buffer[i] = res.light;
	window->pixel_pos_buffer[i] = res.ray.dir;
}

int	raycast(t_level *level, t_window *window, int thread_id)
{
	t_cast_result	res;
	t_ivec2			xy;

	xy.x = thread_id * level->ui.raycast_quality;
	res.raycast_amount = 0;
	while (xy.x < RES_X)
	{
		xy.y = 0;
		while (xy.y < RES_Y)
		{
			if (level->render_is_first_pass
				|| !window->frame_buffer[xy.x + xy.y * RES_X])
			{
				res.ray = ray_set(&level->cam, xy);
				cast_result_set(&res, level);
				cast_all_color(level, &level->ssp[get_ssp(xy)], &res, TRUE);
				set_render_result(window, res, xy.x + xy.y * RES_X);
			}
			xy.y += level->ui.raycast_quality;
		}
		xy.x += THREAD_AMOUNT * level->ui.raycast_quality;
	}
	return (res.raycast_amount);
}
