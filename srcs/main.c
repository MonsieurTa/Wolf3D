/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wta <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/14 09:49:22 by wta               #+#    #+#             */
/*   Updated: 2019/01/14 14:43:30 by wta              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "mlx.h"
#include "wolf3d.h"

int	main(int ac, char **av)
{
	t_map	map_info;
	int		err_id;
	
	err_id = 1;
	if (ac > 1)
		err_id = read_file(av[1], &map_info);
	if (err_id != 1)
		err_handler(err_id);
	return (0);
}
