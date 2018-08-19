#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <string>

namespace kernel
{
	const std::string graphCenter =
		" \n\
		__kernel void graphCenter( \n\
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
				cx[id] = (degree[id] + 1) * x[id]; \n\
				cy[id] = (degree[id] + 1) * y[id]; \n\
				cz[id] = (degree[id] + 1) * z[id]; \n\
			} \n\
		} \n\
		";

	const std::string globalSwing =
		" \n\
		__kernel void globalSwing( \n\
			__const uint n, \n\
			__global float* swgx, \n\
			__global float* swgy, \n\
			__global float* swgz, \n\
			__global float* fcurrx, \n\
			__global float* fcurry, \n\
			__global float* fcurrz, \n\
			__global float* fprevx, \n\
			__global float* fprevy, \n\
			__global float* fprevz, \n\
			__global uint* degree) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				swgx[id] = (degree[id] + 1) * fabs(fcurrx[id] - fprevx[id]); \n\
				swgy[id] = (degree[id] + 1) * fabs(fcurry[id] - fprevy[id]); \n\
				swgz[id] = (degree[id] + 1) * fabs(fcurrz[id] - fprevz[id]); \n\
			} \n\
		} \n\
		";

	const std::string globalTraction =
		" \n\
		__kernel void globalTraction( \n\
			__const uint n, \n\
			__global float* trax, \n\
			__global float* tray, \n\
			__global float* traz, \n\
			__global float* fcurrx, \n\
			__global float* fcurry, \n\
			__global float* fcurrz, \n\
			__global float* fprevx, \n\
			__global float* fprevy, \n\
			__global float* fprevz, \n\
			__global uint* degree) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			if (id < n) \n\
			{ \n\
				trax[id] = (degree[id] + 1) * fabs(fcurrx[id] + fprevx[id]) / 2; \n\
				tray[id] = (degree[id] + 1) * fabs(fcurry[id] + fprevy[id]) / 2; \n\
				traz[id] = (degree[id] + 1) * fabs(fcurrz[id] + fprevz[id]) / 2; \n\
			} \n\
		} \n\
		";

	const std::string sum =
		" \n\
		__kernel void sum( \n\
			__const uint n, \n\
			__global float *graphGlobal, \n\
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
				if (n <= groupSize) \n\
				{ \n\
					graphGlobal[0] = px[0]; \n\
					graphGlobal[1] = py[0]; \n\
					graphGlobal[2] = pz[0]; \n\
				} \n\
				else \n\
				{ \n\
					ox[groupId] = px[0]; \n\
					oy[groupId] = py[0]; \n\
					oz[groupId] = pz[0]; \n\
				} \n\
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
		float forceOfGravity(float kg, float mass, float cp, float p, uint degree) \n\
		{ \n\
			return kg * (cp / mass - p > 0 ? 1 : -1) * (degree + 1); \n\
		} \n\
		\n\
		float strongForceOfGravity(float kg, float mass, float cp, float p, uint degree) \n\
		{ \n\
			return kg * (degree + 1) * (cp / mass - p); \n\
		} \n\
		\n\
		float swing(float fcurr, float fprev, uint degree) \n\
		{ \n\
			return (degree + 1) * fabs(fcurr - fprev); \n\
		} \n\
		\n\
		float localSpeed(float ks, float globalSpeed, float fcurr, float fprev, uint degree) \n\
		{ \n\
			return ks * globalSpeed / ((1 + globalSpeed) * sqrt(swing(fcurr, fprev, degree))); \n\
		} \n\
		\n\
		float localSpeedConstraint(float ksmax, float localSpeed, float fcurr) \n\
		{ \n\
			return (localSpeed < ksmax / fabs(fcurr)) ? localSpeed : (ksmax / fabs(fcurr)); \n\
		} \n\
		\n\
		__kernel void updateNode( \n\
			__const uint n, \n\
			__const float kg, \n\
			__const float graphMass, \n\
			__const uint fg, \n\
			__const uint fsg, \n\
			__const float tau, \n\
			__const float ks, \n\
			__const float ksmax, \n\
			__global float* centerOfMass, \n\
			__global float* globalSwing, \n\
			__global float* globalTraction, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* degree, \n\
			__global float* fcurrx, \n\
			__global float* fcurry, \n\
			__global float* fcurrz, \n\
			__global float* fprevx, \n\
			__global float* fprevy, \n\
			__global float* fprevz) \n\
		{ \n\
			int id = get_global_id(0); \n\
			\n\
			float globalSpeedX = tau * globalTraction[0] / globalSwing[0]; \n\
			float globalSpeedY = tau * globalTraction[1] / globalSwing[1]; \n\
			float globalSpeedZ = tau * globalTraction[2] / globalSwing[2]; \n\
			\n\
			if (id < n) \n\
			{ \n\
				fcurrx[id] += fg ? forceOfGravity(kg, graphMass, centerOfMass[0], x[id], degree[id]) : 0.0; \n\
				fcurry[id] += fg ? forceOfGravity(kg, graphMass, centerOfMass[1], y[id], degree[id]) : 0.0; \n\
				fcurrz[id] += fg ? forceOfGravity(kg, graphMass, centerOfMass[2], z[id], degree[id]) : 0.0; \n\
				\n\
				fcurrx[id] += fsg ? strongForceOfGravity(kg, graphMass, centerOfMass[0], x[id], degree[id]) : 0.0; \n\
				fcurry[id] += fsg ? strongForceOfGravity(kg, graphMass, centerOfMass[1], y[id], degree[id]) : 0.0; \n\
				fcurrz[id] += fsg ? strongForceOfGravity(kg, graphMass, centerOfMass[2], z[id], degree[id]) : 0.0; \n\
				\n\
				float localSpeedX = localSpeed(ks, globalSpeedX, fcurrx[id], fprevx[id], degree[id]); \n\
				float localSpeedY = localSpeed(ks, globalSpeedY, fcurry[id], fprevy[id], degree[id]); \n\
				float localSpeedZ = localSpeed(ks, globalSpeedZ, fcurrz[id], fprevz[id], degree[id]); \n\
				\n\
				localSpeedX = localSpeedConstraint(ksmax, localSpeedX, fcurrx[id]); \n\
				localSpeedY = localSpeedConstraint(ksmax, localSpeedY, fcurry[id]); \n\
				localSpeedZ = localSpeedConstraint(ksmax, localSpeedZ, fcurrz[id]); \n\
				\n\
				x[id] += localSpeedX * fcurrx[id]; \n\
				y[id] += localSpeedY * fcurry[id]; \n\
				z[id] += localSpeedZ * fcurrz[id]; \n\
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
