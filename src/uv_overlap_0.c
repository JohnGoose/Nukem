/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_overlap_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	div_every_uv(t_level *l)
{
	int		i;
	int		j;
	t_vert	*vert;

	i = 0;
	while (i < l->all.tri_amount)
	{
		j = 0;
		while (j < 3 + l->all.tris[i].isquad)
		{
			vert = &l->all.tris[i].verts[j];
			vert->txtr.y = 0.5 + vert->txtr.y / 2;
			j++;
		}
		i++;
	}
}

void	coloring_loop(t_tri *t1, t_bmp *img, t_uv *v)
{
	t_ivec2	pixel;

	while (++v->inc.y < 3)
	{
		v->check.x = v->coord_uv.x + v->precision.x * v->inc.x;
		v->check.y = v->coord_uv.y + v->precision.y * v->inc.y;
		if (point_in_tri(v->check,
				t1->verts[0].txtr, t1->verts[1].txtr, t1->verts[2].txtr)
			|| (t1->isquad && point_in_tri(v->check, t1->verts[3].txtr,
					t1->verts[1].txtr, t1->verts[2].txtr)))
		{
			pixel.x = v->coord.x;
			pixel.y = img->height - v->coord.y;
			wrap_coords(&pixel.x, &pixel.y, img->width, img->height);
			v->og.x = v->coord.x - v->diff.x * img->width;
			v->og.y = (img->height - v->coord.y) + v->diff.y * img->height;
			wrap_coords(&v->og.x, &v->og.y, img->width, img->height);
			img->image[pixel.x + pixel.y * img->width]
				= img->image[v->og.x + v->og.y * img->width];
			return ;
		}
	}
}

void	check_and_color(t_tri *t1, t_bmp *img, t_uv *v)
{
	v->precision.x = 1 / (float)img->width;
	v->precision.y = 1 / (float)img->height;
	v->coord_uv.x = v->coord.x / (float)img->width - v->precision.x;
	v->coord_uv.y = v->coord.y / (float)img->height - v->precision.y;
	v->inc.x = -1;
	while (++v->inc.x < 3)
	{
		v->inc.y = -1;
		coloring_loop(t1, img, v);
	}
}

void	copy_uv2(t_tri *t1, t_uv *v, t_bmp *img)
{
	if (t1->isquad)
		v->max_y = fmax(v->max_y, t1->verts[3].txtr.y);
	v->min_x *= (float)img->width;
	v->max_x *= (float)img->width;
	v->max_y *= (float)img->height;
	v->min_y *= (float)img->height;
	v->coord.x = v->min_x - UV_PADDING;
	while (v->coord.x < v->max_x + UV_PADDING)
	{
		v->coord.y = v->min_y - UV_PADDING;
		while (v->coord.y < v->max_y + UV_PADDING)
		{
			check_and_color(t1, img, v);
			v->coord.y += 1;
		}
		v->coord.x += 1;
	}
}

void	copy_uv(t_tri *t1, t_uv	*v, t_bmp *img)
{
	v->min_x = t1->verts[0].txtr.x;
	v->min_x = fmin(v->min_x, t1->verts[1].txtr.x);
	v->min_x = fmin(v->min_x, t1->verts[2].txtr.x);
	if (t1->isquad)
		v->min_x = fmin(v->min_x, t1->verts[3].txtr.x);
	v->max_x = t1->verts[0].txtr.x;
	v->max_x = fmax(v->max_x, t1->verts[1].txtr.x);
	v->max_x = fmax(v->max_x, t1->verts[2].txtr.x);
	if (t1->isquad)
		v->max_x = fmax(v->max_x, t1->verts[3].txtr.x);
	v->min_y = t1->verts[0].txtr.y;
	v->min_y = fmin(v->min_y, t1->verts[1].txtr.y);
	v->min_y = fmin(v->min_y, t1->verts[2].txtr.y);
	if (t1->isquad)
		v->min_y = fmin(v->min_y, t1->verts[3].txtr.y);
	v->max_y = t1->verts[0].txtr.y;
	v->max_y = fmax(v->max_y, t1->verts[1].txtr.y);
	v->max_y = fmax(v->max_y, t1->verts[2].txtr.y);
	copy_uv2(t1, v, img);
}
