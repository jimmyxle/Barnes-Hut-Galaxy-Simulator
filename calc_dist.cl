__kernel void calc_dist(
	__global float2* in_force,
	__global float4* in_pos,
	__global float4* out_pos)
{
	size_t globalId = get_global_id(0);
	out_pos[globalId].x = 0.5f;
	out_pos[globalId].y = 0.5f;

	size_t localId = get_local_id(0);
	/*
	
	size_t globalId = get_global_id(0);
	//size_t localSize = get_local_size(0);
	//size_t localId = get_local_id(0);


	local_force[globalId] = in_force[globalId];
	local_pos[globalId] = in_pos[globalId];

	//barrier(CLK_LOCAL_MEM_FENCE);

	
	//float2 force = local_force[localId];
	//float4 position = local_pos[localId];

	
		
	//local_pos[localId] = calculate(force, position);
		

	//barrier(CLK_LOCAL_MEM_FENCE);
	

	//out_pos[globalId] = local_pos[localId];
	/**/
}


/*
__kernel void calculate( cl_float2& force,  cl_float4& position)
{
		//timestep per calculation
		float TIME = 0.3f; 
		
			//prevents NaN problems
			if (force.x != force.x)
				force.x = 0;

			if (force.y != force.y)
				force.y = 0;

		//prevents points from accelerating too far from the center
		cl_float max = 1.0f / 25;
		if(force.x >= max)
		{
			force.x = max;
		}
		if (force.x < -max)
		{
			force.x = -max;
		}

		if (force.y >= max)
		{
			force.y = max;
		}
		if (force.y < -max)
		{
			force.y = -max;
		}
		

		//velocities
		//position.z += force.x * TIME;
		//position.w += force.y * TIME;

		//positions
		position.x = 0.5;
		position.y = -0.5;
		
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

/*

__kernel void calc_dist(
__global float2* in_force,
__local  float2* local_force,
__global float4* in_pos,
__local  float4* local_pos,
__global float4* out_pos)
{
*/