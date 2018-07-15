#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <string>

namespace kernel
{
	const std::string nBody =
		" \
		float dist(float x1, float y1, float z1, float x2, float y2, float z2) \
		{ \
			return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)); \
		} \
		\
		float size(uint degree) \
		{ \
			return degree * 0.5 + 0.5; \
		} \
		\
		__kernel void nBody( \
			__const uint n, \
			__const float kr, \
			__const float krp, \
			__global float* x, \
			__global float* y, \
			__global float* z, \
			__global uint* degree, \
			__global float* fx, \
			__global float* fy, \
			__global float* fz) \
		{ \
			uint id = get_global_id(0); \
			\
			if (id < n) \
			{ \
				fx[id] = 0; \
				fy[id] = 0; \
				fz[id] = 0; \
				\
				uint i = (id + 1) < n ? (id + 1) : 0; \
				\
				while (i != id) \
				{ \
					float d = dist(x[id], y[id], z[id], x[i], y[i], z[i]) - size(degree[id]) - size(degree[i]); \
					\
					float degrees = (degree[id] + 1) * (degree[i] + 1); \
					\
					if (d > 0) \
					{ \
						fx[id] += (x[i] - x[id]) / (degree[id] + 1); \
						fy[id] += (y[i] - y[id]) / (degree[id] + 1); \
						fz[id] += (z[i] - z[id]) / (degree[id] + 1); \
						\
						fx[id] += kr * degrees / (x[id] - x[i]); \
						fy[id] += kr * degrees / (y[id] - y[i]); \
						fz[id] += kr * degrees / (z[id] - z[i]); \
					} \
					else if (d < 0) \
					{ \
						fx[id] += krp * degrees * ((x[id] - x[i]) > 0 ? 1 : -1); \
						fy[id] += krp * degrees * ((y[id] - y[i]) > 0 ? 1 : -1); \
						fz[id] += krp * degrees * ((z[id] - z[i]) > 0 ? 1 : -1); \
					} \
					\
					i = (i + 1) < n ? (i + 1) : 0; \
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
}

#endif
