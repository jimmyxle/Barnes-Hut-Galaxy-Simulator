__kernel void calc_dist(
	__global float2* in_force,
	__global float4* in_pos,
	__global float4* out_pos,
	__local float2*  local_force,
	__local float4*  local_pos
)
{
	size_t globalId = get_global_id(0);
	size_t localId = get_local_id(0);

	local_force[localId] = in_force[globalId];
	local_pos[localId] = in_pos[globalId];
	barrier(CLK_LOCAL_MEM_FENCE);

	float TIME = 0.6f;

	float2 force;
	force.x = local_force[localId].x;
	force.y = local_force[localId].y;

	float4 position;
	position.x = local_pos[localId].x;
	position.y = local_pos[localId].y;
	position.z = local_pos[localId].z;
	position.w = local_pos[localId].w;
	
	//velocities
	position.z += force.x * TIME;
	position.w += force.y * TIME;

	//positions
	position.x += force.x*TIME*TIME;
	position.y += force.y*TIME*TIME;

	//bounce particles off the borders

	
	out_pos[globalId].x = position.x;
	out_pos[globalId].y = position.y;
	out_pos[globalId].z = position.z;
	out_pos[globalId].w = position.w;
}

