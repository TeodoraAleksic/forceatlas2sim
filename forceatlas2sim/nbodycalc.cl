__kernel void nbodyCalc(__const uint n, __global float* x, __global float* y, __global float* z, __global uint* degree, 
	__global float* fx, __global float* fy, __global float* fz)
{
	int id = get_global_id(0);

	if (id < n) 
	{
		uint i = n % (id + 1);
		uint count = 0;

		fx[id] = 0;
		fy[id] = 0;
		fz[id] = 0;

		while (count < (n - 1))
		{
			fx[id] += (x[i] - x[id]) + (degree[id] + 1) * (degree[i] + 1) / (x[id] - x[i]);
			fy[id] += (y[i] - y[id]) + (degree[id] + 1) * (degree[i] + 1) / (y[id] - y[i]);
			fz[id] += (z[i] - z[id]) + (degree[id] + 1) * (degree[i] + 1) / (z[id] - z[i]);

			i = n % (i + 1);
			++count;
		}
	}
}
