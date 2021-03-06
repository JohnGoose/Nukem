/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_arr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuikka <vkuikka@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 18:14:24 by vkuikka           #+#    #+#             */
/*   Updated: 2019/12/02 18:40:01 by vkuikka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_arr(int **array, unsigned arr_len, unsigned str_len)
{
	unsigned int	x;
	unsigned int	y;

	x = 0;
	y = 0;
	while (y < arr_len)
	{
		x = 0;
		while (x < str_len)
		{
			ft_putnbr(array[y][x]);
			write(1, " ", 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}
