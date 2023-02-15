/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_read_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	set_uv_vert(char **file, int i, t_vec2 *vert)
{
	int	j;

	j = 0;
	while (file[i][j] != ' ')
	{
		j++;
	}
	j++;
	vert->x = ft_atof(&file[i][j]);
	while (file[i][j] != ' ')
	{
		j++;
	}
	j++;
	vert->y = ft_atof(&file[i][j]);
}

t_vec2	*load_uvs(char **file)
{
	int		uv_amount;
	t_vec2	*res;
	int		j;
	int		i;

	i = 0;
	uv_amount = 0;
	res = NULL;
	while (file[i])
		if (!ft_strncmp(file[i++], "vt ", 3))
			uv_amount++;
	if (!uv_amount)
		return (NULL);
	res = (t_vec2 *)malloc(sizeof(t_vec2) * uv_amount);
	if (!res)
		ft_error("memory allocation failed\n");
	i = 0;
	j = 0;
	while (file[i])
	{
		if (!ft_strncmp(file[i], "vt ", 3))
			set_uv_vert(file, i, &res[j++]);
		i++;
	}
	return (res);
}

void	set_vert(char **file, int i, t_vec3 *vert)
{
	int	j;

	j = 0;
	while (file[i][j] != ' ')
	{
		j++;
	}
	j++;
	vert->x = ft_atof(&file[i][j]);
	while (file[i][j] != ' ')
	{
		j++;
	}
	j++;
	vert->y = ft_atof(&file[i][j]);
	while (file[i][j] != ' ')
	{
		j++;
	}
	j++;
	vert->z = ft_atof(&file[i][j]);
}

t_vec3	*load_verts(char **file)
{
	int		ver_amount;
	t_vec3	*res;
	int		j;
	int		i;

	i = 0;
	ver_amount = 0;
	while (file[i])
		if (!ft_strncmp(file[i++], "v ", 2))
			ver_amount++;
	if (!ver_amount)
		return (NULL);
	res = (t_vec3 *)malloc(sizeof(t_vec3) * ver_amount);
	if (!res)
		ft_error("memory allocation failed\n");
	i = 0;
	j = 0;
	while (file[i])
	{
		if (!ft_strncmp(file[i], "v ", 2))
			set_vert(file, i, &res[j++]);
		i++;
	}
	return (res);
}

void	read_uv_indices(char *str, t_ivec3 *uv_index, int *j, int x)
{
	(*j)++;
	if (x == 0)
		uv_index->x = ft_atoi(&str[*j]) - 1;
	else if (x == 1)
		uv_index->y = ft_atoi(&str[*j]) - 1;
	else if (x == 2)
		uv_index->z = ft_atoi(&str[*j]) - 1;
	while (str[*j] && str[*j] != ' ')
		(*j)++;
}
