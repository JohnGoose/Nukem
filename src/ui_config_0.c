/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_config_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 16:54:13 by vkuikka           #+#    #+#             */
/*   Updated: 2022/02/07 19:40:03 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int	get_fps(void)
{
	struct timeval	time;
	static long		s;
	static int		frames;
	static int		fps;

	frames++;
	gettimeofday(&time, NULL);
	if (s != time.tv_sec)
	{
		s = time.tv_sec;
		fps = frames;
		frames = 0;
	}
	return (fps);
}

int	get_selected_amount(t_level *level)
{
	int	selected_amount;
	int	i;

	i = 0;
	selected_amount = 0;
	while (i < level->all.tri_amount)
	{
		if (level->all.tris[i].selected)
			selected_amount++;
		i++;
	}
	return (selected_amount);
}

void	copy_tri_settings(t_tri *a, t_tri *b)
{
	a->isbreakable = b->isbreakable;
	a->isgrid = b->isgrid;
	a->opacity = b->opacity;
	a->opacity_precise = b->opacity_precise;
	a->reflectivity = b->reflectivity;
	a->refractivity = b->refractivity;
}

void	ui_config_projectile_settings(t_projectile *projectile)
{
	char	buf[100];

	ft_sprintf(buf,
		"projectile speed: %.1fm/s (0 = no projectile)",
		projectile->speed);
	float_slider(&projectile->speed, buf, 0, 50);
	ft_sprintf(buf, "projectile scale: %.2f",
		projectile->scale);
	float_slider(&projectile->scale,
		buf, 0.1, 5);
	ft_sprintf(buf, "distance limit: %.1fm",
		projectile->dist);
	float_slider(&projectile->dist, buf, 1, 10);
	ft_sprintf(buf, "attack damage: %.1f",
		projectile->damage);
	float_slider(&projectile->damage, buf, 0, 50);
}

void	ui_config_enemy_settings(t_enemy_settings *enemy)
{
	char	buf[100];

	ft_sprintf(buf, "move speed: %.1fm/s",
		enemy->move_speed);
	float_slider(&enemy->move_speed, buf, 0, 10);
	ft_sprintf(buf, "attack frequency: %.2f seconds per attack",
		enemy->attack_frequency / 1000.0);
	float_slider(&enemy->attack_frequency, buf, 0, 5000);
	ft_sprintf(buf, "melee range: %.1fm",
		enemy->melee_range);
	float_slider(&enemy->melee_range, buf, 0.01, 10);
	ft_sprintf(buf, "move time: %.1fm",
		enemy->move_duration);
	float_slider(&enemy->move_duration, buf, 0.01, 10);
	ft_sprintf(buf, "shoot time: %.1fm",
		enemy->shoot_duration);
	float_slider(&enemy->shoot_duration, buf, 0, 10);
}
