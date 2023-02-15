/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_overlap_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 15:33:12 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	move_uv_y(t_tri *t1, t_level *l, t_uv *v, int i)
{
	if (t1->isquad)
		t1->verts[3].txtr.y -= v->max - v->min;
	v->diff.y -= v->max - v->min;
	while (tri_uv_intersect(*t1, l->all.tris[i]))
	{
		t1->verts[0].txtr.y -= 0.01;
		t1->verts[1].txtr.y -= 0.01;
		t1->verts[2].txtr.y -= 0.01;
		if (t1->isquad)
			t1->verts[3].txtr.y -= 0.01;
		v->diff.y -= 0.01;
	}
}

void	clear_intersection(t_tri *t1, t_level *l, t_uv *v, int i)
{
	v->max = t1->verts[0].txtr.y;
	v->max = fmax(v->max, t1->verts[1].txtr.y);
	v->max = fmax(v->max, t1->verts[2].txtr.y);
	if (t1->isquad)
		v->max = fmax(v->max, t1->verts[3].txtr.y);
	v->min = t1->verts[0].txtr.y;
	v->min = fmin(v->min, t1->verts[1].txtr.y);
	v->min = fmin(v->min, t1->verts[2].txtr.y);
	if (t1->isquad)
		v->min = fmin(v->min, t1->verts[3].txtr.y);
	t1->verts[0].txtr.y -= v->max - v->min;
	t1->verts[1].txtr.y -= v->max - v->min;
	t1->verts[2].txtr.y -= v->max - v->min;
	move_uv_y(t1, l, v, i);
}

void	baked_state(t_level *l, t_uv *v)
{
	if (!l->spray_overlay)
		ft_error("memory allocation failed");
	l->baked = (t_color *)ft_realloc(l->baked,
			sizeof(t_color) * (l->texture.width * l->texture.height),
			sizeof(t_color)
			* (2 * l->texture.width * l->texture.height));
	if (!l->baked)
		ft_error("memory allocation failed");
	l->texture.height *= 2;
	l->normal_map.height *= 2;
	div_every_uv(l);
	v->diff.y /= 2.0;
}

void	loop_state(t_level *l, t_uv *v)
{
	l->texture.image = (int *)ft_realloc(l->texture.image,
			sizeof(int) * (l->texture.width * l->texture.height),
			sizeof(int) * (2 * l->texture.width * l->texture.height));
	if (!l->texture.image)
		ft_error("memory allocation failed");
	l->normal_map.image = (int *)ft_realloc(l->normal_map.image,
			sizeof(int) * (l->normal_map.width * l->normal_map.height),
			sizeof(int)
			* (2 * l->normal_map.width * l->normal_map.height));
	if (!l->normal_map.image)
		ft_error("memory allocation failed");
	l->spray_overlay = (unsigned *)ft_realloc(l->spray_overlay,
			sizeof(unsigned) * (l->texture.width * l->texture.height),
			sizeof(unsigned)
			* (2 * l->texture.width * l->texture.height));
	baked_state(l, v);
}

void	move_uv(t_tri *t1, int t1_index, t_level *l, t_uv *v)
{
	int		i;

	i = -1;
	v->diff.y = 0;
	v->diff.x = 0;
	while (++i < l->all.tri_amount)
	{
		if (i == t1_index)
			continue ;
		if (tri_uv_intersect(*t1, l->all.tris[i]))
		{
			clear_intersection(t1, l, v, i);
			i = -1;
		}
		if (t1->verts[0].txtr.y < 0 || t1->verts[1].txtr.y < 0
			|| t1->verts[2].txtr.y < 0
			|| (t1->isquad && t1->verts[3].txtr.y < 0))
			loop_state(l, v);
	}
	if (v->diff.y != 0.0 || v->diff.x != 0.0)
	{
		copy_uv(t1, v, &l->texture);
		copy_uv(t1, v, &l->normal_map);
	}
}
