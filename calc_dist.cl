__kernel void calc_dist(__global float* in_force, __local float* local_force,
 __global float* in_pos, __local float* local_pos,
__global float* out_pos)
{
	/*
	size_t globalId = get_global_id(0);
	size_t localSize = get_local_size(0);
	size_t localId = get_local_id(0);


	local_force[localId] = in_force[globalId];
	local_pos[localId] = in_pos[globalId];

	barrier(CLK_LOCAL_MEM_FENCE);


		Vector2D force = local_force[localId];
		Vector2D current_position = local_position[localId];

		double TIME = 0.3; 
		
			//prevents NaN problems
			if (force.x != force.x)
				force.x = 0;
			
			if (force.y != force.y)
				force.y = 0;
		

		double acc_x = force.x ;
		double acc_y = force.y ;
		//prevents points from accelerating too far from the center
		double max = 1.0 / 25;
		if (acc_x >= max)
		{
			acc_x = max;
		}
		if (acc_x < -max)
		{
			acc_x = -max;
		}

		if (acc_y >= max)
		{
			acc_y = max;
		}
		if (acc_y < -max)
		{
			acc_y = -max;
		}

		//velocities
		current_position.xy->vx += acc_x * TIME;
		current_position.xy->vy += acc_y * TIME;

		//positions
		current_position.xy->x = 0.5;
		current_position.xy->y = -0.5;
		
	//	current_position.xy->x += current_position.xy->vx;
	//	current_position.xy->y += current_position.xy->vy;
		//bounce particles off the borders
		if (current_position.xy->x >= 0.99)
		{
			current_position.xy->x = 0.99;
			current_position.xy->vx *= -0.5;
		}
		if (current_position.xy->x <= -0.99)
		{
			current_position.xy->x = -0.99;
			current_position.xy->vx *= -0.5;
		}
		if (current_position.xy->y >= 0.99)
		{
			current_position.xy->y = 0.99;
			current_position.xy->vy *= -0.5;
		}
		if (current_position.xy->y <= -0.99)
		{
			current_position.xy->y = -0.99;
			current_position.xy->vy *= -0.5;
		}

		
		//local_position[i] = calculate(local_force[i], local_position[i]);
		
		barrier(CLK_LOCAL_MEM_FENCE);
	

	out_force[globalId] = localData[localId];
	/**/
}


/*
__kernel Vector2D calculate( Vector2D force,  Vector2D position)
{
		Vector2D current_position = position;
		//timestep per calculation
		double TIME = 0.3; 
		{
			//prevents NaN problems
			if (force.x != force.x)
				force.x = 0;

			if (force.y != force.y)
				force.y = 0;
		}

		double acc_x = force.x ;
		double acc_y = force.y ;
		//prevents points from accelerating too far from the center
		double max = 1.0 / 25;
		if (acc_x >= max)
		{
			acc_x = max;
		}
		if (acc_x < -max)
		{
			acc_x = -max;
		}

		if (acc_y >= max)
		{
			acc_y = max;
		}
		if (acc_y < -max)
		{
			acc_y = -max;
		}

		//velocities
		current_position.xy->vx += acc_x * TIME;
		current_position.xy->vy += acc_y * TIME;

		//positions
		current_position.xy->x = 0.5;
		current_position.xy->y = -0.5;
		
	//	current_position.xy->x += current_position.xy->vx;
	//	current_position.xy->y += current_position.xy->vy;

		//bounce particles off the borders
		if (current_position.xy->x >= 0.99)
		{
			current_position.xy->x = 0.99;
			current_position.xy->vx *= -0.5;
		}
		if (current_position.xy->x <= -0.99)
		{
			current_position.xy->x = -0.99;
			current_position.xy->vx *= -0.5;
		}
		if (current_position.xy->y >= 0.99)
		{
			current_position.xy->y = 0.99;
			current_position.xy->vy *= -0.5;
		}
		if (current_position.xy->y <= -0.99)
		{
			current_position.xy->y = -0.99;
			current_position.xy->vy *= -0.5;
		}

		return current_postion;
}

*/