#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <string>

namespace kernel
{
	const std::string nBody =
		" \n\
		float size(uint degree) \n\
		{ \n\
			return degree * 0.5 + 0.5; \n\
		} \n\
		\n\
		float dist(float p1, float p2, uint degree1, uint degree2) \n\
		{ \n\
			return fabs(p1 - p2) - size(degree1) - size(degree2); \n\
		} \n\
		\n\
		float fa(float p1, float p2, uint degree1, uint degree2) \n\
		{ \n\
			if (dist(p1, p2, degree1, degree2) > 0) \n\
				return (p2 - p1) / (degree1 + 1); \n\
			else \n\
				return 0; \n\
		} \n\
		\n\
		float fr(__const float kr, __const float kpr, float p1, float p2, uint degree1, uint degree2) \n\
		{ \n\
			float d = dist(p1, p2, degree1, degree2); \n\
		\n\
			if (d > 0) \n\
				return kr * (degree1 + 1) * (degree2 + 1) / (p1 - p2); \n\
			else if (d < 0) \n\
				return kpr * (degree1 + 1) * (degree2 + 1) * ((p1 - p2) >= 0 ? 1 : -1); \n\
			else \n\
				return 0; \n\
		} \n\
		\n\
		__kernel void nBody( \n\
			__const uint n, \n\
			__const float kr, \n\
			__const float krp, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz) \n\
		{ \n\
			uint id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				fx[id] = 0; \n\
				fy[id] = 0; \n\
				fz[id] = 0; \n\
				\n\
				uint i = (id + 1) < n ? (id + 1) : 0; \n\
				\n\
				while (i != id) \n\
				{ \n\
					fx[id] += fa(x[id], x[i], degree[id], degree[i]); \n\
					fx[id] += fr(kr, krp, x[id], x[i], degree[id], degree[i]); \n\
					\n\
					fy[id] += fa(y[id], y[i], degree[id], degree[i]); \n\
					fy[id] += fr(kr, krp, y[id], y[i], degree[id], degree[i]); \n\
					\n\
					fz[id] += fa(z[id], z[i], degree[id], degree[i]); \n\
					fz[id] += fr(kr, krp, z[id], z[i], degree[id], degree[i]); \n\
					\n\
					i = (i + 1) < n ? (i + 1) : 0; \n\
				} \n\
			} \n\
		} \n\
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
