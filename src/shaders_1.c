/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/02 15:28:28 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	reflection(t_cast_result *res, t_level *l, t_obj *obj)
{
	t_cast_result	reflection;
	t_ray			normal;

	reflection = *res;
	normal.pos = res->ray.pos;
	normal.dir = res->normal;
	vec_mult(&normal.dir, vec_dot(reflection.ray.dir, normal.dir) * -2);
	vec_add(&reflection.ray.dir, reflection.ray.dir, normal.dir);
	vec_normalize(&reflection.ray.dir);
	reflection.dist = 0;
	cast_all_color(l, obj, &reflection, TRUE);
	res->color = crossfade((unsigned int)res->color >> 8, reflection.color >> 8,
			l->all.tris[res->face_index].reflectivity * 0xff,
			(unsigned int)res->color << 24 >> 24);
}

char	*init_rule30(void)
{
	char	*cells;
	int		x;
	int		y;
	int		i;

	cells = (char *)malloc(sizeof(char) * RULE_30_SIZE * RULE_30_SIZE);
	if (!cells)
		ft_error("memory allocation failed");
	ft_bzero(cells, RULE_30_SIZE * RULE_30_SIZE);
	cells[RULE_30_SIZE / 2] = 1;
	y = -1;
	while (++y < RULE_30_SIZE - 1)
	{
		x = 0;
		while (++x < RULE_30_SIZE - 1)
		{
			i = y * RULE_30_SIZE + x;
			if ((cells[i - 1] == 1 && cells[i] == 0 && cells[i + 1] == 0)
				|| (cells[i - 1] == 0 && cells[i] == 1)
				|| (cells[i - 1] == 0 && cells[i] == 0 && cells[i + 1] == 1))
				cells[((y + 1) * RULE_30_SIZE) + x] = 1;
		}
	}
	return (cells);
}

unsigned int	shader_rule30(t_vec3 pos)
{
	static char	*cells = NULL;
	int			res;
	int			x;
	int			y;

	if (!cells)
		cells = init_rule30();
	res = 0xffffffff;
	x = (int)(pos.x + RULE_30_SIZE / 2);
	y = (int)(pos.z);
	if (x > 0 && x < RULE_30_SIZE - 1 && pos.z >= 0.0 && y < RULE_30_SIZE - 1
		&& cells[x + y * RULE_30_SIZE])
		res = 0x000000ff;
	return (res);
}
