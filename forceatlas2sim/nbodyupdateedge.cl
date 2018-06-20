__kernel void nbodyUpdateEdge(__const uint n, __global float* x, __global float* y, __global float* z,
	__global uint* sid, __global float* sx, __global float* sy, __global float* sz,
	__global uint* tid, __global float* tx, __global float* ty, __global float* tz)
{
	int id = get_global_id(0);

	if (id < n)
	{
		sx[id] = x[sid[id]];
		sy[id] = y[sid[id]];
		sz[id] = z[sid[id]];

		tx[id] = x[tid[id]];
		ty[id] = y[tid[id]];
		tz[id] = z[tid[id]];
	}
}
