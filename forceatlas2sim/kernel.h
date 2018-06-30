#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <string>

const std::string nBody =
" \
__kernel void nBody(__const uint n, __global float* x, __global float* y, __global float* z, __global uint* degree, \
	__global float* fx, __global float* fy, __global float* fz) \
{ \
	int id = get_global_id(0); \
\
	if (id < n) \
	{ \
		uint i = n % (id + 1); \
		uint count = 0; \
\
		fx[id] = 0; \
		fy[id] = 0; \
		fz[id] = 0; \
\
		while (count < (n - 1)) \
		{ \
			fx[id] += (x[i] - x[id]) + (degree[id] + 1) * (degree[i] + 1) / (x[id] - x[i]); \
			fy[id] += (y[i] - y[id]) + (degree[id] + 1) * (degree[i] + 1) / (y[id] - y[i]); \
			fz[id] += (z[i] - z[id]) + (degree[id] + 1) * (degree[i] + 1) / (z[id] - z[i]); \
\
			i = n % (i + 1); \
			++count; \
		} \
	} \
} \
";

const std::string updateNode =
" \
__kernel void updateNode(__const uint n, __global float* x, __global float* y, __global float* z, __global uint* degree, \
	__global float* fx, __global float* fy, __global float* fz) \
{ \
	int id = get_global_id(0); \
\
	if (id < n) \
	{ \
		x[id] += 0.01 * fx[id] / (degree[id] + 1); \
		y[id] += 0.01 * fy[id] / (degree[id] + 1); \
		z[id] += 0.01 * fz[id] / (degree[id] + 1); \
	} \
} \
";

const std::string updateEdge = 
" \
__kernel void updateEdge(__const uint n, __global float* x, __global float* y, __global float* z, \
	__global uint* sid, __global float* sx, __global float* sy, __global float* sz, \
	__global uint* tid, __global float* tx, __global float* ty, __global float* tz) \
{ \
	int id = get_global_id(0); \
 \
	if (id < n) \
	{ \
		sx[id] = x[sid[id]]; \
		sy[id] = y[sid[id]]; \
		sz[id] = z[sid[id]]; \
\
		tx[id] = x[tid[id]]; \
		ty[id] = y[tid[id]]; \
		tz[id] = z[tid[id]]; \
	} \
} \
";

#endif