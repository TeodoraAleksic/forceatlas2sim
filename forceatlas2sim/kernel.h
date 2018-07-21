#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <string>

namespace kernel
{
	const std::string initCenter =
		" \n\
		__kernel void initCenter( \n\
			__const uint n, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* cx, \n\
			__global float* cy, \n\
			__global float* cz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
			} \n\
		} \n\
		";


	const std::string calcCenter =
		" \n\
		__kernel void calcCenter( \n\
			__const uint n, \n\
			__global float *ix, \n\
			__global float *iy, \n\
			__global float *iz, \n\
			__global float *ox, \n\
			__global float *oy, \n\
			__global float *oz, \n\
			__local float *px, \n\
			__local float *py, \n\
			__local float *pz) \n\
		{ \n\
			uint globalId = get_global_id(0); \n\
			uint localId = get_local_id(0); \n\
			uint groupSize = get_local_size(0); \n\
			uint groupId = get_group_id(0); \n\
			\n\
			px[localId] = globalId < n ? ix[globalId] : 0.0; \n\
			py[localId] = globalId < n ? iy[globalId] : 0.0; \n\
			pz[localId] = globalId < n ? iz[globalId] : 0.0; \n\
			\n\
			barrier(CLK_LOCAL_MEM_FENCE); \n\
			\n\
			for (uint stride = groupSize / 2; stride > 0; stride /= 2) \n\
			{ \n\
				if (localId < stride) \n\
				{ \n\
					px[localId] += px[localId + stride]; \n\
					py[localId] += py[localId + stride]; \n\
					pz[localId] += pz[localId + stride]; \n\
				} \n\
				\n\
				barrier(CLK_LOCAL_MEM_FENCE); \n\
			} \n\
			\n\
			if (localId == 0) \n\
			{ \n\
				ox[groupId] = px[0]; \n\
				oy[groupId] = py[0]; \n\
				oz[groupId] = pz[0]; \n\
			} \n\
		} \n\
		";

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
		" \n\
		__kernel void updateNode( \n\
			__const uint n, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				x[id] += 0.01 * fx[id] / (degree[id] + 1); \n\
				y[id] += 0.01 * fy[id] / (degree[id] + 1); \n\
				z[id] += 0.01 * fz[id] / (degree[id] + 1); \n\
			} \n\
		} \n\
		";

	const std::string updateNodeFg =
		" \n\
		__kernel void updateNodeFg( \n\
			__const uint n, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz, \n\
			__const float kg, \n\
			__const float mass, \n\
			__global float* cx, \n\
			__global float* cy, \n\
			__global float* cz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				fx[id] += kg * (cx[0] / mass - x[id] > 0 ? 1 : -1) * (degree[id] + 1); \n\
				fy[id] += kg * (cy[0] / mass - y[id] > 0 ? 1 : -1) * (degree[id] + 1); \n\
				fz[id] += kg * (cz[0] / mass - z[id] > 0 ? 1 : -1) * (degree[id] + 1); \n\
				\n\
				x[id] += fx[id] / (degree[id] + 1); \n\
				y[id] += fy[id] / (degree[id] + 1); \n\
				z[id] += fz[id] / (degree[id] + 1); \n\
			} \n\
		} \n\
		";

	const std::string updateNodeFsg =
		" \n\
		__kernel void updateNodeFsg( \n\
			__const uint n, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz, \n\
			__const float kg, \n\
			__const float mass, \n\
			__global float* cx, \n\
			__global float* cy, \n\
			__global float* cz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				fx[id] += kg * (degree[id] + 1) * (cx[0] / mass - x[id]); \n\
				fy[id] += kg * (degree[id] + 1) * (cy[0] / mass - y[id]); \n\
				fz[id] += kg * (degree[id] + 1) * (cz[0] / mass - z[id]); \n\
				\n\
				x[id] += fx[id] / (degree[id] + 1); \n\
				y[id] += fy[id] / (degree[id] + 1); \n\
				z[id] += fz[id] / (degree[id] + 1); \n\
			} \n\
		} \n\
		";

	const std::string updateEdge =
		" \n\
		__kernel void updateEdge( \n\
			__const uint n, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* sid, \n\
			__global float* sx, \n\
			__global float* sy, \n\
			__global float* sz, \n\
			__global uint* tid, \n\
			__global float* tx, \n\
			__global float* ty, \n\
			__global float* tz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				sx[id] = x[sid[id]]; \n\
				sy[id] = y[sid[id]]; \n\
				sz[id] = z[sid[id]]; \n\
				\n\
				tx[id] = x[tid[id]]; \n\
				ty[id] = y[tid[id]]; \n\
				tz[id] = z[tid[id]]; \n\
			} \n\
		} \n\
		";
}

#endif
