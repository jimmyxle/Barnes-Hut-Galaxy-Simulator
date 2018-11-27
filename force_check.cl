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
		
	float max = 25.0f;
	
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
/*
//prevents NaN problems
if (forces[i].x != forces[i].x)
forces[i].x = 0;

if (forces[i].y != forces[i].y)
forces[i].y = 0;

//prevents points from accelerating too far from the center
float max = 1.0 / 2;
if (forces[i].x >= max)
{
forces[i].x = max;
}
if (forces[i].x < -max)
{
forces[i].x = -max;
}

if (forces[i].y >= max)
{
forces[i].y = max;
}
if (forces[i].y < -max)
{
forces[i].y = -max;
}

forces_copy[i].x = forces[i].x;
forces_copy[i].y = forces[i].y;


*/