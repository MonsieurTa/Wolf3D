/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/16 03:57:58 by wta               #+#    #+#             */
/*   Updated: 2019/01/17 18:25:49 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mlx.h"
#include "wolf3d.h"

void	get_ray_dir(int x, t_vec2 *ray_dir, t_info *info)
{
	t_player	*player;
	double		proj_x;

	player = &info->player;
	proj_x = 2. * (double)x / (double)SCREEN_W - 1.;
	*ray_dir = (t_vec2){player->dir.x + player->cam_dir.x * proj_x,
						player->dir.y + player->cam_dir.y * proj_x};
}

double	dda(int *side, t_vec2 *ray_dir, t_info *info)
{
	t_vec2		curr_pos;
	t_vec2		delta;
	t_vec2		step;
	t_vec2		side_dist;
	double		dist;

	delta = (t_vec2){fabs(1. / ray_dir->x), fabs(1. / ray_dir->y)};
	curr_pos = (t_vec2){(int)info->player.pos.x, (int)info->player.pos.y};
	side_dist = (t_vec2){ (ray_dir->x >= 0.) ? ((curr_pos.x - info->player.pos.x
	+ 1) * delta.x) : ((info->player.pos.x - curr_pos.x) * delta.x), (ray_dir->y
	>= 0.) ? ((curr_pos.y - info->player.pos.y + 1) * delta.y) :
	((info->player.pos.y - curr_pos.y) * delta.y)};
	step = (t_vec2){(ray_dir->x >= 0.) ? 1 : -1, (ray_dir->y >= 0.) ? 1 : -1};
	while (1)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta.x;
			curr_pos.x += step.x;
			*side = 0;
		}
		else
		{
			side_dist.y += delta.y;
			curr_pos.y += step.y;
			*side = 1;
		}
		if (info->m_info.map[(int)curr_pos.y][(int)curr_pos.x] > '0'
			&& info->m_info.map[(int)curr_pos.y][(int)curr_pos.x] != '@')
			break ;
	}
	if (*side == 0)
		dist = (curr_pos.x - info->player.pos.x + (1. - step.x) / 2.) / ray_dir->x;
	else
		dist = (curr_pos.y - info->player.pos.y + (1. - step.y) / 2.) / ray_dir->y;
	return (dist);
}

void	draw_line(int x, int side, double dist, t_info *info, t_vec2 ray_dir)
{
	int	start;
	int	end;
	int	idx;
	int line_h;

	line_h = (int)(SCREEN_H / dist);
	if ((start = SCREEN_H / 2 - line_h / 2) < 0)
		start = 0;
	if ((end = SCREEN_H / 2 + line_h / 2) >= SCREEN_H)
		end = SCREEN_H - 1;
	idx = -1;
	while (++idx < start)
		info->mlx.img_str[x + (idx * info->mlx.sizel / 4)] = 0xb2b2ff;

	// #### WALL ########################################################

	unsigned int	tex_x;
	t_img			texture;

	tex_x = !side ? ((info->player.pos.y + dist * ray_dir.y) * (1 << 30)) :
						((info->player.pos.x + dist * ray_dir.x) * (1 << 30));
	tex_x %= (1 << 30);
	if (side == 0)
		texture = (ray_dir.x > 0) ? info->m_info.textures[0] : info->m_info.textures[1];
	else
		texture = (ray_dir.y < 0) ? info->m_info.textures[2] : info->m_info.textures[3];
	tex_x = (!side && ray_dir.x > 0) || (side && ray_dir.y < 0) ?
			texture.width - (double)tex_x * texture.width / (1 << 30) - 1 : (double)tex_x * texture.width / (1 << 30);
	while (idx < end)
	{
		int tex_y = (((idx * 512 - SCREEN_H * 256 + line_h * 256)) * texture.height) / (line_h * 512);
		info->mlx.img_str[x + (idx * info->mlx.sizel / 4)] = texture.img_str[texture.width * tex_y + tex_x];
		idx++;
	}
	// #############################################################


	// ######## FLOOR #######################
	t_vec2			floor_wall;
	double			wall_x;

	texture = info->m_info.textures[0];

	wall_x = !side ? info->player.pos.y + dist * ray_dir.y :
						info->player.pos.x + dist * ray_dir.x;
	wall_x -= floor(wall_x);

    //   if (drawEnd < 0) drawEnd = h; //becomes < 0 when the integer overflows

    //   //draw the floor from drawEnd to the bottom of the screen
    //   for(int y = drawEnd + 1; y < h; y++)
    //   {
    //     currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead

    //     double weight = (currentDist - distPlayer) / (distWall - distPlayer);

    //     double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
    //     double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

    //     int floorTexX, floorTexY;
    //     floorTexX = int(currentFloorX * texWidth) % texWidth;
    //     floorTexY = int(currentFloorY * texHeight) % texHeight;

    //     //floor
    //     buffer[y][x] = (texture[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
    //     //ceiling (symmetrical!)
    //     buffer[h - y][x] = texture[6][texWidth * floorTexY + floorTexX];
    //   }
    // }

	t_int2 map = (t_int2){map.x = info->player.pos.x + ray_dir.x * dist,
				info->player.pos.y + ray_dir.y * dist};
	//if (ray_dir.x >)

	if (!side)
		floor_wall = ray_dir.x > 0 ?
		(t_vec2){map.x, map.y + wall_x} :
		(t_vec2){map.x, map.y + wall_x};
	else
		floor_wall = ray_dir.y > 0 ?
		(t_vec2){map.x + wall_x, map.y} :
		(t_vec2){map.x + wall_x, map.y};

	double currentDist;

	while (idx < SCREEN_H - 1)
	{
		currentDist = SCREEN_H / (2. * idx - SCREEN_H);

		double weight = currentDist / dist;

		t_vec2 current_floor = vec2_add(vec2_multf(floor_wall, weight), vec2_multf(info->player.pos, (1.0 - weight)));

		t_int2 floor_tex;
		floor_tex.x = (int)(current_floor.x * texture.width) % texture.width;
		floor_tex.y = (int)(current_floor.y * texture.height) % texture.height;
		if (floor_tex.x < 0)
			floor_tex.x = 0;
			if (floor_tex.y < 0)
			floor_tex.y = 0;

		info->mlx.img_str[x + (idx * info->mlx.sizel / 4)] = info->m_info.textures[3].img_str[info->m_info.textures[3].width * floor_tex.y + floor_tex.x];
		info->mlx.img_str[x + ((SCREEN_H - idx) * info->mlx.sizel / 4)] = info->m_info.textures[2].img_str[info->m_info.textures[2].width * floor_tex.y + floor_tex.x];
		++idx;
	}

	// idx = end - 1;
	// while (++idx < SCREEN_H - 1)
	// 	info->mlx.img_str[x + (idx * info->mlx.sizel / 4)] = 0xf4a460;

	// #####################################
}

void	raycasting(t_info *info)
{
	t_vec2	ray_dir;
	double	dist;
	int		screen_x;
	int		side;

	screen_x = -1;
	info->player.cam_dir = vec2_normalize(rotate2d(info->player.dir, M_PI / 2.));
	while (++screen_x < SCREEN_W)
	{
		side = -1;
		get_ray_dir(screen_x, &ray_dir, info);
		dist = dda(&side, &ray_dir, info);
		draw_line(screen_x, side, dist, info, ray_dir);
	}
}
