__kernel void nbody(__const uint n, __global float* x, __global float* y, __global float* z, __global uint* degree)
{
	int id = get_global_id(0);

	if (id < n) {
		uint i = n % (id + 1);
		uint count = 0;

		float fx = 0;
		float fy = 0;
		float fz = 0;

		while (count < (n - 1))
		{
			fx += (x[i] - x[id]) + (degree[id] + 1) * (degree[i] + 1) / (x[id] - x[i]);
			fy += (y[i] - y[id]) + (degree[id] + 1) * (degree[i] + 1) / (y[id] - y[i]);
			fz += (z[i] - z[id]) + (degree[id] + 1) * (degree[i] + 1) / (z[id] - z[i]);

			i = n % (i + 1);
			++count;
		}

		x[id] += 0.01 * fx / (degree[id] + 1);
		y[id] += 0.01 * fy / (degree[id] + 1);
		z[id] += 0.01 * fz / (degree[id] + 1);
	}
}
