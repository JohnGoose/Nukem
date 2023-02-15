/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wireframe_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	render_wireframe(unsigned int *texture, t_level *level, t_obj *obj,
														int is_visible)
{
	t_vec3	ss_vert;
	int		i;
	int		k;

	i = -1;
	while (++i < obj->tri_amount)
	{
		if (!level->ui.wireframe && (is_visible || !obj->tris[i].selected))
			continue ;
		k = 0;
		while (k < 3 + obj->tris[i].isquad)
		{
			ss_vert = wireframe_render_line(obj,
					(t_ivec3){i, k, is_visible}, level, texture);
			if (!is_visible)
				find_closest_mouse(&ss_vert, &i, &k, &level->ui.state.mouse);
			k++;
		}
		if (is_visible || !level->ui.wireframe_culling_visual)
			put_normal(texture, level, obj->tris[i], WF_VISIBLE_NORMAL_COL);
		else
			put_normal(texture, level, obj->tris[i], WF_NORMAL_COL);
	}
}

void	wireframe(unsigned int *texture, t_level *level)
{
	int	x;
	int	y;

	if (!level->ui.wireframe_on_top && level->ui.wireframe)
	{
		y = -1;
		while (++y < RES_Y)
		{
			x = -1;
			while (++x < RES_X)
				texture[x + (y * RES_X)] = WF_BACKGROUND_COL;
		}
	}
	if (level->ui.wireframe && level->ui.wireframe_culling_visual)
		render_wireframe(texture, level, &level->visible, TRUE);
	render_wireframe(texture, level, &level->all, FALSE);
}
