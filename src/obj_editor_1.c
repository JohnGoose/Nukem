/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_editor_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/06 00:11:41 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_new_face(t_tri *tri, t_vec3 pos, t_vec3 dir, float scale)
{
	t_vec3	tri_avg;

	ft_bzero(tri, sizeof(t_tri));
	tri_avg = dir;
	tri->isquad = TRUE;
	vec_mult(&tri_avg, 2);
	vec_add(&tri_avg, pos, tri_avg);
	set_new_face_pos(tri, tri_avg, scale);
	tri_optimize(tri, false);
}

void	add_face(t_level *level)
{
	free_culling(level);
	level->all.tri_amount++;
	level->all.tris = (t_tri *)ft_realloc(level->all.tris,
			sizeof(t_tri) * level->all.tri_amount - 1,
			sizeof(t_tri) * level->all.tri_amount);
	if (!level->all.tris)
		ft_error("memory allocation failed");
	set_new_face(&level->all.tris[level->all.tri_amount - 1],
		level->cam.pos, level->cam.front, 1);
	level->all.tris[level->all.tri_amount - 1].index
		= level->all.tri_amount - 1;
	init_culling(level);
}

void	remove_tri(t_level *level, int i)
{
	int	k;

	k = i - 1;
	while (++k < level->all.tri_amount - 1)
	{
		level->all.tris[k] = level->all.tris[k + 1];
		level->all.tris[k].index = k;
	}
}

void	remove_faces(t_level *level)
{
	int	amount;
	int	original_amount;
	int	i;

	free_culling(level);
	amount = level->all.tri_amount;
	original_amount = level->all.tri_amount;
	i = level->all.tri_amount - 1;
	while (i >= 0)
	{
		if (level->all.tris[i].selected)
		{
			amount--;
			remove_tri(level, i);
		}
		i--;
	}
	level->all.tri_amount = amount;
	level->all.tris = (t_tri *)ft_realloc(level->all.tris,
			sizeof(t_tri) * original_amount, sizeof(t_tri) * amount);
	if (!level->all.tris)
		ft_error("memory allocation failed");
	init_culling(level);
}
