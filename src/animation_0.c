/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/08 18:18:32 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	obj_check_polygon_equality(t_obj *a, t_obj *b)
{
	int	i;

	if (a->tri_amount != b->tri_amount)
		return (FALSE);
	i = 0;
	while (i < a->tri_amount)
	{
		if (a->tris[i].isquad != b->tris[i].isquad)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	load_animation(char *get_filename, t_obj_animation *animation,
							int amount, float duration)
{
	char			filename[100];
	unsigned char	*mem;
	int				i;

	animation->keyframe_amount = amount;
	animation->keyframes = (t_obj *)malloc(sizeof(t_obj)
			* animation->keyframe_amount);
	i = 0;
	while (i < amount)
	{
		ft_sprintf(filename, "%s_%06d.obj", get_filename, i);
		mem = read_embedded_file(filename);
		load_obj_from_memory(mem, &animation->keyframes[i]);
		if (!obj_check_polygon_equality(&animation->keyframes[0],
				&animation->keyframes[i]))
			ft_error("animation frames not equal polygons");
		free(mem);
		i++;
	}
	animation->loop = TRUE;
	animation->duration_multiplier = 1.0;
	animation->duration = duration;
	return (TRUE);
}

void	obj_copy(t_obj *target, t_obj *source)
{
	int	i;

	i = 0;
	target->tri_amount = source->tri_amount;
	while (i < source->tri_amount)
	{
		target->tris[i] = source->tris[i];
		i++;
	}
}

t_obj	get_animation_target(t_obj_animation *animation)
{
	t_obj	res;

	res.tri_amount = animation->keyframes[0].tri_amount;
	res.tris = (t_tri *)malloc(sizeof(t_tri) * res.tri_amount);
	if (!res.tris)
		ft_error("animation malloc fail");
	obj_copy(&res, &animation->keyframes[0]);
	return (res);
}

void	tri_lerp(t_tri *target, t_tri *a, t_tri *b, float t)
{
	int	i;

	i = 0;
	*target = *a;
	while (i < 3 + target->isquad)
	{
		target->verts[i].pos
			= vec_interpolate(a->verts[i].pos, b->verts[i].pos, t);
		i++;
	}
}
