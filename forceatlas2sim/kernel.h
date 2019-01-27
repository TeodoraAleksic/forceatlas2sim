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
			__global uint* nodeDegree, \n\
			__global float* cx, \n\
			__global float* cy, \n\
			__global float* cz) \n\
		{ \n\
			int id = get_global_id(0); \n\
		 \n\
			if (id < n) \n\
			{ \n\
				// Calculates partial center of graph \n\
				cx[id] = (nodeDegree[id] + 1) * x[id]; \n\
				cy[id] = (nodeDegree[id] + 1) * y[id]; \n\
				cz[id] = (nodeDegree[id] + 1) * z[id]; \n\
			} \n\
		 }\n\
		";

	const std::string globalSwing =
		" \n\
		__kernel void globalSwing( \n\
			__const uint n, \n\
			__global float* swingx, \n\
			__global float* swingy, \n\
			__global float* swingz, \n\
			__global float* fxCurr, \n\
			__global float* fyCurr, \n\
			__global float* fzCurr, \n\
			__global float* fxPrev, \n\
			__global float* fyPrev, \n\
			__global float* fzPrev, \n\
			__global uint* nodeDegree) \n\
		{ \n\
			int id = get_global_id(0); \n\
		 \n\
			if (id < n) \n\
			{ \n\
				// Calculates global swing of node \n\
				swingx[id] = (nodeDegree[id] + 1) * fabs(fxCurr[id] - fxPrev[id]); \n\
				swingy[id] = (nodeDegree[id] + 1) * fabs(fyCurr[id] - fyPrev[id]); \n\
				swingz[id] = (nodeDegree[id] + 1) * fabs(fzCurr[id] - fzPrev[id]); \n\
			} \n\
		 }\n\
		";

	const std::string globalTraction =
		" \n\
		__kernel void globalTraction( \n\
			__const uint n, \n\
			__global float* tracx, \n\
			__global float* tracy, \n\
			__global float* tracz, \n\
			__global float* fxCurr, \n\
			__global float* fyCurr, \n\
			__global float* fzCurr, \n\
			__global float* fxPrev, \n\
			__global float* fyPrev, \n\
			__global float* fzPrev, \n\
			__global uint* nodeDegree) \n\
		{ \n\
			int id = get_global_id(0); \n\
		 \n\
			if (id < n) \n\
			{ \n\
				// Calculates global traction of node \n\
				tracx[id] = (nodeDegree[id] + 1) * fabs(fxCurr[id] + fxPrev[id]) / 2; \n\
				tracy[id] = (nodeDegree[id] + 1) * fabs(fyCurr[id] + fyPrev[id]) / 2; \n\
				tracz[id] = (nodeDegree[id] + 1) * fabs(fzCurr[id] + fzPrev[id]) / 2; \n\
			} \n\
		 }\n\
		";

	const std::string sum =
		" \n\
		__kernel void sum( \n\
			__const uint numOfNodes, \n\
			__global float *graphGlobal, \n\
			__global float *inx, \n\
			__global float *iny, \n\
			__global float *inz, \n\
			__global float *outx, \n\
			__global float *outy, \n\
			__global float *outz, \n\
			__local float *posx, \n\
			__local float *posy, \n\
			__local float *posz) \n\
		{ \n\
			uint globalId = get_global_id(0); \n\
			uint localId = get_local_id(0); \n\
			uint groupSize = get_local_size(0); \n\
			uint groupId = get_group_id(0); \n\
		 \n\
			// Copies global node value to local memory \n\
			posx[localId] = globalId < numOfNodes ? inx[globalId] : 0.0; \n\
			posy[localId] = globalId < numOfNodes ? iny[globalId] : 0.0; \n\
			posz[localId] = globalId < numOfNodes ? inz[globalId] : 0.0; \n\
		 \n\
			// Waits for all compute units in wavefront to finish \n\
			barrier(CLK_LOCAL_MEM_FENCE); \n\
		 \n\
			// Calculates sum of local memory array \n\
			for (uint stride = groupSize / 2; stride > 0; stride /= 2) \n\
			{ \n\
				if (localId < stride) \n\
				{ \n\
					posx[localId] += posx[localId + stride]; \n\
					posy[localId] += posy[localId + stride]; \n\
					posz[localId] += posz[localId + stride]; \n\
				} \n\
			 \n\
				barrier(CLK_LOCAL_MEM_FENCE); \n\
			} \n\
		 \n\
			// First compute unit writes sum result \n\
			if (localId == 0) \n\
			{ \n\
				if (numOfNodes <= groupSize) \n\
				{ \n\
					graphGlobal[0] = posx[0]; \n\
					graphGlobal[1] = posy[0]; \n\
					graphGlobal[2] = posz[0]; \n\
				} \n\
				else \n\
				{ \n\
					outx[groupId] = posx[0]; \n\
					outy[groupId] = posy[0]; \n\
					outz[groupId] = posz[0]; \n\
				} \n\
			} \n\
		 } \n\
		";

	const std::string forceAttr =
		" \n\
		int findEdge( \n\
			uint numOfEdges, \n\
			uint nodeId1, \n\
			uint nodeId2, \n\
			__global uint* sourceNodeId, \n\
			__global uint* targetNodeId, \n\
			__global int* sourceNodeOffset) \n\
		{ \n\
			// Checks if the first node is the source of any branch \n\
			if (sourceNodeOffset[nodeId1] != -1) \n\
			{ \n\
				int i = sourceNodeOffset[nodeId1]; \n\
			 \n\
				// Goes through branches where the first node is the source node \n\
				while (i < numOfEdges && sourceNodeId[i] == nodeId1) \n\
				{ \n\
					// Looks for the second node as the target node of the branch \n\
					if (targetNodeId[i] == nodeId2) break; \n\
					++i; \n\
				} \n\
			 \n\
				if (i < numOfEdges && sourceNodeId[i] == nodeId1 && targetNodeId[i] == nodeId2) \n\
					return i; \n\
				else \n\
					return -1; \n\
			} \n\
			else \n\
				return -1; \n\
		} \n\
		float findWeight( \n\
			uint numOfEdges, \n\
			uint nodeId1, \n\
			uint nodeId2, \n\
			__global uint* sourceNodeId, \n\
			__global uint* targetNodeId, \n\
			__global int* sourceNodeOffset, \n\
			__global float* edgeWeight) \n\
		{ \n\
			// Tries to find if the first and the second node share a branch \n\
			int i = findEdge(numOfEdges, nodeId1, nodeId2, sourceNodeId, targetNodeId, sourceNodeOffset); \n\
			i = (i == -1) ? findEdge(numOfEdges, nodeId2, nodeId1, sourceNodeId, targetNodeId, sourceNodeOffset) : i; \n\
			return (i == -1) ? 0 : edgeWeight[i]; \n\
		} \n\
		 \n\
		float getNodeSize(uint nodeDegree) \n\
		{ \n\
			// Calculates the real size of a node \n\
			return nodeDegree * 0.5 + 0.5; \n\
		} \n\
		 \n\
		float getNodeDistance(float nodePos1, float nodePos2, uint nodeDegree1, uint nodeDegree2) \n\
		{ \n\
			// Calculates the distance between the surfaces of two nodes \n\
			return fabs(nodePos1 - nodePos2) - getNodeSize(nodeDegree1) - getNodeSize(nodeDegree2); \n\
		} \n\
		 \n\
		float fa(__const int delta, float nodePos1, float nodePos2, uint nodeDegree1, uint nodeDegree2, float edgeWeight) \n\
		{ \n\
			// Calculates the attraction force between two nodes if they don't overlap \n\
			if (getNodeDistance(nodePos1, nodePos2, nodeDegree1, nodeDegree2) > 0) \n\
				return pow(edgeWeight, delta) * (nodePos2 - nodePos1) / (nodeDegree1 + 1); \n\
			else \n\
				return 0; \n\
		} \n\
		 \n\
		__kernel void forceAttr( \n\
			__const uint numOfNodes, \n\
			__const uint numOfEdges, \n\
			__const int delta, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* nodeDegree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz, \n\
			__global uint* sourceNodeId, \n\
			__global uint* targetNodeId, \n\
			__global int* sourceNodeOffset, \n\
			__global float* edgeWeight) \n\
		{ \n\
			uint id = get_global_id(0); \n\
		 \n\
			if (id < numOfNodes) \n\
			{ \n\
				fx[id] = 0; \n\
				fy[id] = 0; \n\
				fz[id] = 0; \n\
			 \n\
				uint i = (id + 1) < numOfNodes ? (id + 1) : 0; \n\
			 \n\
				// Calculates force of attraction between the current node and all other nodes \n\
				while (i != id) \n\
				{ \n\
					// Finds weight of edge between two nodes (if they share an edge) \n\
					float weight = findWeight(numOfEdges, id, i, sourceNodeId, targetNodeId, sourceNodeOffset, edgeWeight); \n\
				 \n\
					// Calculates force of attraction for all three axes \n\
					fx[id] += fa(delta, x[id], x[i], nodeDegree[id], nodeDegree[i], weight); \n\
					fy[id] += fa(delta, y[id], y[i], nodeDegree[id], nodeDegree[i], weight); \n\
					fz[id] += fa(delta, z[id], z[i], nodeDegree[id], nodeDegree[i], weight); \n\
				 \n\
					i = (i + 1) < numOfNodes ? (i + 1) : 0; \n\
				} \n\
			} \n\
		} \n\
		";

	const std::string forceRepl =
		" \n\
		float getNodeSize(uint nodeDegree) \n\
		{ \n\
			// Calculates the real size of a node \n\
			return nodeDegree * 0.5 + 0.5; \n\
		} \n\
		 \n\
		float getNodeDistance(float nodePos1, float nodePos2, uint nodeDegree1, uint nodeDegree2) \n\
		{ \n\
			// Calculates the distance between the surfaces of two nodes \n\
			return fabs(nodePos1 - nodePos2) - getNodeSize(nodeDegree1) - getNodeSize(nodeDegree2); \n\
		} \n\
		 \n\
		float fr(__const float kr, __const float kpr, float nodePos1, float nodePos2, uint nodeDegree1, uint nodeDegree2) \n\
		{ \n\
			float dist = getNodeDistance(nodePos1, nodePos2, nodeDegree1, nodeDegree2); \n\
		 \n\
			// Calculates the repulsion force between two nodes based on the distance between their edges \n\
			if (dist > 0) \n\
				return kr * (nodeDegree1 + 1) * (nodeDegree2 + 1) / (nodePos1 - nodePos2); \n\
			else if (dist < 0) \n\
				return kpr * (nodeDegree1 + 1) * (nodeDegree2 + 1) * ((nodePos1 - nodePos2) >= 0 ? 1 : -1); \n\
			else \n\
				return 0; \n\
		} \n\
		 \n\
		__kernel void forceRepl( \n\
			__const uint numOfNodes, \n\
			__const float kr, \n\
			__const float krp, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* nodeDegree, \n\
			__global float* fx, \n\
			__global float* fy, \n\
			__global float* fz) \n\
		{ \n\
			uint id = get_global_id(0); \n\
		 \n\
			if (id < numOfNodes) \n\
			{ \n\
				uint i = (id + 1) < numOfNodes ? (id + 1) : 0; \n\
			 \n\
				// Calculates force of repulsion between the current node and all other nodes \n\
				while (i != id) \n\
				{ \n\
					// Calculates force of repulsion for all three axes \n\
					fx[id] += fr(kr, krp, x[id], x[i], nodeDegree[id], nodeDegree[i]); \n\
					fy[id] += fr(kr, krp, y[id], y[i], nodeDegree[id], nodeDegree[i]); \n\
					fz[id] += fr(kr, krp, z[id], z[i], nodeDegree[id], nodeDegree[i]); \n\
				 \n\
					i = (i + 1) < numOfNodes ? (i + 1) : 0; \n\
				} \n\
			} \n\
		} \n\
		";

	const std::string updateNode =
		" \n\
		float forceOfGravity(float kg, float graphMass, float graphCenterOfMass, float nodePosition, uint nodeDegree) \n\
		{ \n\
			return kg * (graphCenterOfMass / graphMass - nodePosition > 0 ? 1 : -1) * (nodeDegree + 1); \n\
		} \n\
		 \n\
		float strongForceOfGravity(float kg, float graphMass, float graphCenterOfMass, float nodePosition, uint nodeDegree) \n\
		{ \n\
			return kg * (nodeDegree + 1) * (graphCenterOfMass / graphMass - nodePosition); \n\
		} \n\
		 \n\
		float localSwing(float fcurr, float fprev, uint nodeDegree) \n\
		{ \n\
			return (nodeDegree + 1) * fabs(fcurr - fprev); \n\
		} \n\
		 \n\
		float localSpeed(float ks, float globalSpeed, float fcurr, float fprev, uint nodeDegree) \n\
		{ \n\
			return ks * globalSpeed / ((1 + globalSpeed) * sqrt(localSwing(fcurr, fprev, nodeDegree))); \n\
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
			__global float* graphCenterOfMass, \n\
			__global float* globalSwing, \n\
			__global float* globalTraction, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* nodeDegree, \n\
			__global float* fcurrx, \n\
			__global float* fcurry, \n\
			__global float* fcurrz, \n\
			__global float* fprevx, \n\
			__global float* fprevy, \n\
			__global float* fprevz) \n\
		{ \n\
			int id = get_global_id(0); \n\
		 \n\
			// Calcualte graph global speed \n\
			float globalSpeedX = tau * globalTraction[0] / globalSwing[0]; \n\
			float globalSpeedY = tau * globalTraction[1] / globalSwing[1]; \n\
			float globalSpeedZ = tau * globalTraction[2] / globalSwing[2]; \n\
		 \n\
			if (id < n) \n\
			{ \n\
				// Add force of gravity if specified \n\
				fcurrx[id] += fg ? forceOfGravity(kg, graphMass, graphCenterOfMass[0], x[id], nodeDegree[id]) : 0.0; \n\
				fcurry[id] += fg ? forceOfGravity(kg, graphMass, graphCenterOfMass[1], y[id], nodeDegree[id]) : 0.0; \n\
				fcurrz[id] += fg ? forceOfGravity(kg, graphMass, graphCenterOfMass[2], z[id], nodeDegree[id]) : 0.0; \n\
			 \n\
				// Add strong force of gravity if specified \n\
				fcurrx[id] += fsg ? strongForceOfGravity(kg, graphMass, graphCenterOfMass[0], x[id], nodeDegree[id]) : 0.0; \n\
				fcurry[id] += fsg ? strongForceOfGravity(kg, graphMass, graphCenterOfMass[1], y[id], nodeDegree[id]) : 0.0; \n\
				fcurrz[id] += fsg ? strongForceOfGravity(kg, graphMass, graphCenterOfMass[2], z[id], nodeDegree[id]) : 0.0; \n\
			 \n\
				// Calculate the local speed of the node \n\
				float localSpeedX = localSpeed(ks, globalSpeedX, fcurrx[id], fprevx[id], nodeDegree[id]); \n\
				float localSpeedY = localSpeed(ks, globalSpeedY, fcurry[id], fprevy[id], nodeDegree[id]); \n\
				float localSpeedZ = localSpeed(ks, globalSpeedZ, fcurrz[id], fprevz[id], nodeDegree[id]); \n\
			 \n\
				// Constrain the local speed to a maximum value \n\
				localSpeedX = localSpeedConstraint(ksmax, localSpeedX, fcurrx[id]); \n\
				localSpeedY = localSpeedConstraint(ksmax, localSpeedY, fcurry[id]); \n\
				localSpeedZ = localSpeedConstraint(ksmax, localSpeedZ, fcurrz[id]); \n\
		 \n\
				// Calculate the new node position \n\
				x[id] += localSpeedX * fcurrx[id]; \n\
				y[id] += localSpeedY * fcurry[id]; \n\
				z[id] += localSpeedZ * fcurrz[id]; \n\
			} \n\
		 } \n\
		";

	const std::string updateEdge =
		" \n\
		__kernel void updateEdge( \n\
			__const uint numOfNodes, \n\
			__global float* x, \n\
			__global float* y, \n\
			__global float* z, \n\
			__global uint* sourceNodeId, \n\
			__global float* sourcex, \n\
			__global float* sourcey, \n\
			__global float* sourcez, \n\
			__global uint* targetNodeId, \n\
			__global float* targetx, \n\
			__global float* targety, \n\
			__global float* targetz) \n\
		{ \n\
			int id = get_global_id(0); \n\
		 \n\
			// Updates the source and target coordinates of an edge \n\
			if (id < numOfNodes) \n\
			{ \n\
				sourcex[id] = x[sourceNodeId[id]]; \n\
				sourcey[id] = y[sourceNodeId[id]]; \n\
				sourcez[id] = z[sourceNodeId[id]]; \n\
			 \n\
				targetx[id] = x[targetNodeId[id]]; \n\
				targety[id] = y[targetNodeId[id]]; \n\
				targetz[id] = z[targetNodeId[id]]; \n\
			} \n\
		 } \n\
		";
}

#endif
