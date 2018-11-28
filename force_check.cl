__kernel void force_check(
	__global float2* in_force,
	__global float2* out_force
)
{
	
	size_t globalId = get_global_id(0);
	
	float2 temp_force;
	temp_force.x = in_force[globalId].x;
	temp_force.y = in_force[globalId].y;
	
		
	if (temp_force.x != temp_force.x)
		temp_force.x = 0;
	if (temp_force.y != temp_force.y)
		temp_force.y = 0;
		
	float max = 1.0f/25.0f;
	
	if (temp_force.x >= max)
		temp_force.x = max;

	if (temp_force.x < -max)
		temp_force.x = -max;

	if (temp_force.y >= max)
		temp_force.y = max;

	if (temp_force.y < -max)
		temp_force.y = -max;
		
	out_force[globalId].x = temp_force.x;
	out_force[globalId].y = temp_force.y;
	/**/
}