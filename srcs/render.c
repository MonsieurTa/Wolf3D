/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 18:05:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/15 19:05:45 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include <math.h>

int		cast_ray(const int x, const int y, const t_info *env)
{
	t_vec2			ray_dir;
	int				pos[2];
	double			vec_nxt_sqr_fact[2];
	double			unt_nxt_sqr_dist[2];

	ray_dir = (t_vec2) //ray direction / point on the 'screen' in world coords
	{
		(2.0 * (x + 0.5) / (float)env->res[0] - 1.0) * tan(env->player.fov) * (16./9.),
		0
	};
	ray_dir = vec2_normalize(ray_dir);

	t_vec2 vec_step;
	vec_step.x = fabs(1 / ray_dir.x);
	vec_step.y = fabs(1 / ray_dir.y);

	t_vec2 square_dist;
	int step_sign[2];

	if (ray_dir.x < 0)
	{
		step_sign[0] = -1;
		square_dist.x = (env->player.pos.x - floor(env->player.pos.x)) * vec_step.x;
	}
	else
	{
		step_sign[0] = 1;
		square_dist.x = (floor(env->player.pos.x) - env->player.pos.x + 1.) * vec_step.x;
	}
	if (ray_dir.y < 0)
	{
		step_sign[1] = -1;
		square_dist.y = (env->player.pos.y - floor(env->player.pos.y)) * vec_step.y;
	}
	else
	{
		step_sign[0] = 1;
		square_dist.y = (floor(env->player.pos.y) - env->player.pos.y + 1.) * vec_step.y;
	}

	t_vec2 step_pos = env->player.pos;
	float wall_dst = __FLT_MAX__;
	while (1)
	{
		if (square_dist[0] < square_dist[1])
		{
			square_dist[0] +=
		}
		if (env->m_info.map[(int)step_pos.x][(int)step_pos.y] == 1)
		{
			wall_dst = vec2_mod(vec2_sub(step_pos, env->player.pos));
			break ;
		}
		vec2_multf(step_pos, stepsize);
	}




	return (trace_ray(env, (t_ray){env->camera.org, screen_point}, 2));
}

void		render(t_info *env)
{
	int				u;
	int				v;
	unsigned long	px_id;

	//should mlx calls be protected?
	env->mlx.img_ptr = mlx_new_image(env->mlx.mlx_ptr, env->res[0], env->res[1]);
	env->mlx.img_str = (int *)mlx_get_data_addr(env->mlx.img_ptr, &env->mlx.bpp, &env->mlx.sizel, &env->mlx.endian);

	px_id = 0;
	v = -1;
	while (++v < env->res[1])
	{
		u = -1;
		while (++u < env->res[0])
		{
			((int*)env->mlx.img_str)[px_id++] = cast_ray(u, v, env);
		}
	}
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, env->mlx.img_ptr, 0, 0);
	mlx_destroy_image(env->mlx.mlx_ptr, env->mlx.img_ptr);
}