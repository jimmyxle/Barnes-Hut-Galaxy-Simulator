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

	//velocities
	local_pos[localId].z += local_force[localId].x * TIME;
	local_pos[localId].w += local_force[localId].y * TIME;

	//positions
	local_pos[localId].x += local_pos[localId].z * TIME;
	local_pos[localId].y += local_pos[localId].w * TIME;

	out_pos[globalId].x = local_pos[localId].x;
	out_pos[globalId].y = local_pos[localId].y;
	out_pos[globalId].z = local_pos[localId].z;
	out_pos[globalId].w = local_pos[localId].w;
}
/*
	
		
	float2 force;
	force.x = local_force[localId].x;
	force.y = local_force[localId].y;

	float4 position;
	position.x = local_pos[localId].x;
	position.y = local_pos[localId].y;
	position.z = local_pos[localId].z;
	position.w = local_pos[localId].w;
	
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
	position.x += position.z * TIME;
	position.y += position.w * TIME;

	//bounce particles off the borders

	
	out_pos[globalId].x = position.x;
	out_pos[globalId].y = position.y;
	out_pos[globalId].z = position.z;
	out_pos[globalId].w = position.w;
*/
