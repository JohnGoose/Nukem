/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_read_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	read_indices(char *str, t_ivec3 *tex_index, t_ivec3 *uv_index)
{
	int		j;
	int		x;

	*tex_index = (t_ivec3){0, 0, 0};
	*uv_index = (t_ivec3){0, 0, 0};
	j = 0;
	while (str[j] != ' ')
		j++;
	j++;
	x = -1;
	while (++x < 3)
	{
		if (x == 0)
			tex_index->x = ft_atoi(&str[j]) - 1;
		else if (x == 1)
			tex_index->y = ft_atoi(&str[j]) - 1;
		else if (x == 2)
			tex_index->z = ft_atoi(&str[j]) - 1;
		while (str[j] && ft_isdigit(str[j]))
			j++;
		if (str[j] == '/')
			read_uv_indices(str, uv_index, &j, x);
		j++;
	}
	return (str[j - 1] == '\0');
}

int	set_tri(char *str, t_vec3 *verts, t_vec2 *uvs, t_tri *tri)
{
	t_ivec3	tex_index;
	t_ivec3	uv_index;

	if (!read_indices(str, &tex_index, &uv_index))
		return (FALSE);
	if (uvs)
	{
		tri->verts[0].txtr.x = uvs[uv_index.x].x;
		tri->verts[0].txtr.y = uvs[uv_index.x].y;
		tri->verts[1].txtr.x = uvs[uv_index.y].x;
		tri->verts[1].txtr.y = uvs[uv_index.y].y;
		tri->verts[2].txtr.x = uvs[uv_index.z].x;
		tri->verts[2].txtr.y = uvs[uv_index.z].y;
	}
	tri->verts[0].pos.x = verts[tex_index.x].x;
	tri->verts[0].pos.y = -verts[tex_index.x].y;
	tri->verts[0].pos.z = -verts[tex_index.x].z;
	tri->verts[1].pos.x = verts[tex_index.y].x;
	tri->verts[1].pos.y = -verts[tex_index.y].y;
	tri->verts[1].pos.z = -verts[tex_index.y].z;
	tri->verts[2].pos.x = verts[tex_index.z].x;
	tri->verts[2].pos.y = -verts[tex_index.z].y;
	tri->verts[2].pos.z = -verts[tex_index.z].z;
	return (TRUE);
}

int	obj_set_all_tris_res(t_vec3 *verts, t_vec2 *uvs, int res)
{
	free(verts);
	if (uvs)
		free(uvs);
	if (res == FALSE)
		nonfatal_error("obj not triangulated");
	return (res);
}

int	obj_set_all_tris(char **file, t_obj *obj)
{
	int		i;
	int		j;
	t_vec3	*verts;
	t_vec2	*uvs;

	verts = load_verts(file);
	if (!verts)
		return (FALSE);
	uvs = load_uvs(file);
	if (!uvs)
		nonfatal_error("obj has no uv");
	i = 0;
	j = 0;
	while (file[i])
	{
		if (file[i][0] == 'f' && file[i][1] == ' '
			&& !set_tri(file[i], verts, uvs, &obj->tris[j++]))
			return (obj_set_all_tris_res(verts, uvs, FALSE));
		i++;
	}
	return (obj_set_all_tris_res(verts, uvs, TRUE));
}

int	obj_get_face_amount(char **file)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (file[i])
	{
		if (!ft_strncmp(file[i], "f ", 2))
			res++;
		i++;
	}
	return (res);
}
