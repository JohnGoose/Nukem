/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_read_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpehkone <rpehkone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/06/06 00:11:58 by rpehkone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	tri_optimize(t_tri *tri, int is_dynamic)
{
	vec_sub(&tri->v0v2, tri->verts[1].pos, tri->verts[0].pos);
	vec_sub(&tri->v0v1, tri->verts[2].pos, tri->verts[0].pos);
	vec_cross(&tri->normal, tri->v0v2, tri->v0v1);
	vec_normalize(&tri->normal);
	tri->dynamic = is_dynamic;
}

int	load_obj_internal(char **file, t_obj *obj)
{
	int		i;

	i = 0;
	obj->tri_amount = obj_get_face_amount(file);
	obj->tris = (t_tri *)malloc(sizeof(t_tri) * obj->tri_amount);
	if (!obj->tris || !obj->tri_amount)
		return (FALSE);
	ft_bzero(obj->tris, sizeof(t_tri) * obj->tri_amount);
	if (!obj_set_all_tris(file, obj))
		return (FALSE);
	find_quads(obj);
	i = 0;
	while (i < obj->tri_amount)
	{
		obj->tris[i].index = i;
		tri_optimize(&obj->tris[i], false);
		i++;
	}
	return (TRUE);
}

int	load_obj(char *filename, t_obj *obj)
{
	char	**file;
	int		res;

	file = file2d(filename);
	if (!file)
		return (FALSE);
	res = TRUE;
	if (!load_obj_internal(file, obj))
		res = FALSE;
	free_file2d(file);
	return (res);
}

void	load_obj_from_memory(unsigned char *data,
					t_obj *obj)
{
	char	**file;

	file = file2d_from_memory(data);
	if (!load_obj_internal(file, obj))
		ft_error("obj memory read fail");
	free_file2d(file);
}
