/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wireframe_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2021/10/11 18:58:59 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	pixel_put_force(int x, int y, int color, unsigned int *texture)
{
	if (x < 0 || y < 0 || x >= RES_X || y >= RES_Y)
		return ;
	if (texture[x + (y * RES_X)] != WF_SELECTED_COL)
		texture[x + (y * RES_X)] = color;
}

void	put_vertex(t_vec3 vertex, int color, unsigned int *texture)
{
	int	a;
	int	b;

	if (vertex.z < 0)
		return ;
	a = -1;
	while (a < 2)
	{
		b = -1;
		while (b < 2)
		{
			pixel_put_force(vertex.x + a, vertex.y + b, color, texture);
			b++;
		}
		a++;
	}
}

void	camera_offset(t_vec3 *vertex, t_camera *cam)
{
	vertex->x -= cam->pos.x;
	vertex->y -= cam->pos.y;
	vertex->z -= cam->pos.z;
	rotate_vertex(-cam->look_side, vertex, 0);
	rotate_vertex(-cam->look_up, vertex, 1);
	vertex->x /= vertex->z / (RES_X / cam->fov_x);
	vertex->y /= vertex->z / (RES_Y / cam->fov_y);
	vertex->x += RES_X / 2.0;
	vertex->y += RES_Y / 2.0;
}

void	put_normal(unsigned int *texture, t_level *level, t_tri tri,
							unsigned int color)
{
	t_vec3	normal;
	t_vec3	avg;
	int		amount;
	int		j;

	amount = 3 + tri.isquad;
	avg = (t_vec3){0, 0, 0};
	j = 0;
	while (j < amount)
	{
		avg.x += tri.verts[j].pos.x;
		avg.y += tri.verts[j].pos.y;
		avg.z += tri.verts[j].pos.z;
		j++;
	}
	vec_div(&avg, amount);
	normal = tri.normal;
	normal.x = avg.x + normal.x * WF_NORMAL_LEN;
	normal.y = avg.y + normal.y * WF_NORMAL_LEN;
	normal.z = avg.z + normal.z * WF_NORMAL_LEN;
	camera_offset(&avg, &level->cam);
	camera_offset(&normal, &level->cam);
	put_vertex(avg, 0, texture);
	print_line(avg, normal, color, texture);
}

t_vec3	wireframe_render_line(t_obj *obj, t_ivec3 i, t_level *level,
									unsigned int *texture)
{
	static int	arr[4] = {1, 3, 0, 2};
	t_vec3		start;
	t_vec3		stop;
	int			next;

	next = (i.y + 1) % 3;
	if (obj->tris[i.x].isquad)
		next = arr[i.y];
	start = obj->tris[i.x].verts[i.y].pos;
	stop = obj->tris[i.x].verts[next].pos;
	camera_offset(&start, &level->cam);
	camera_offset(&stop, &level->cam);
	if (obj->tris[i.x].selected)
		print_line(start, stop, WF_SELECTED_COL, texture);
	else if (i.z)
		print_line(start, stop, WF_VISIBLE_COL, texture);
	else if (level->ui.show_quads && !obj->tris[i.x].isquad)
		print_line(start, stop, WF_NOT_QUAD_WARNING_COL, texture);
	else
		print_line(start, stop, WF_UNSELECTED_COL, texture);
	if (obj->tris[i.x].verts[i.y].selected)
		put_vertex(start, WF_SELECTED_COL, texture);
	else
		put_vertex(start, WF_VERT_COL, texture);
	return (start);
}
