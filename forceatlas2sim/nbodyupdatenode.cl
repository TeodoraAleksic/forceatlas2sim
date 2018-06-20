__kernel void nbodyUpdateNode(__const uint n, __global float* x, __global float* y, __global float* z, __global uint* degree,
	__global float* fx, __global float* fy, __global float* fz)
{
	int id = get_global_id(0);

	if (id < n)
	{
		x[id] += 0.01 * fx[id] / (degree[id] + 1);
		y[id] += 0.01 * fy[id] / (degree[id] + 1);
		z[id] += 0.01 * fz[id] / (degree[id] + 1);
	}
}
