/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wta <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 15:23:42 by wta               #+#    #+#             */
/*   Updated: 2019/01/15 18:57:11 by wta              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "wolf3d.h"

void	set_cam_pos(t_info *info)
{
	t_player	*tmp;
	t_vec2		pos;

	tmp = &info->player;
}

void	set_cam_dir(t_info *info)
{
	t_vec2	*ptr;

	ptr = &info->player.c_dir;
	*ptr = vec2_normalize(rotate2d(info->player.p_dir), -M_PI / 2);
}

void	set_dist(t_info *info)
{
	t_player	*tmp;

	tmp = &info->player;
	tmp->dist = tan(180. - 90. - (tmp->fov / 2.));
}

void	cast_ray(int x, t_info *info)
{

	t_vec2	raydir;
	double	screen_x;

	screen_x = 2. * x / SCREEN_W - 1;
	raydir = vec2_add(info->player.p_dir, )
}

void	raytracing(t_info *info)
{
	int	x;

	x = -1;
	set_cam_dir(info);
	while (++x < SCREEN_W)
	{
		cast_ray(x, info);
	}
}
