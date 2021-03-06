#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

namespace shader
{
	const std::string nodeVert =
	" \
	#version 330 core \n\
	\n\
	layout(location = 0) in vec3 position; \n\
	layout(location = 1) in float offsetX; \n\
	layout(location = 2) in float offsetY; \n\
	layout(location = 3) in float offsetZ; \n\
	layout(location = 4) in uint scale; \n\
	\n\
	uniform mat4 projection; \n\
	uniform mat4 view; \n\
	uniform mat4 model; \n\
	\n\
	uniform mat3 normalMatrix; \n\
	\n\
	flat out int instanceId; \n\
	out vec3 vertPos; \n\
	out vec3 vertNormal; \n\
	\n\
	void main() \n\
	{ \n\
		instanceId = gl_InstanceID; \n\
		\n\
		vec3 offset = vec3(offsetX, offsetY, offsetZ); \n\
		vec3 newPos = position * (float(scale) * 0.5 + 0.5) + offset; \n\
		\n\
		vertPos = vec3(model * vec4(newPos, 1.0f)); \n\
		vertNormal = normalMatrix * normalize(position); \n\
		\n\
		gl_Position = projection * view * model * vec4(newPos, 1); \n\
	} \n\
	";

	const std::string nodeFrag =
	" \
	#version 330 core \n\
	\n\
	flat in int instanceId; \n\
	in vec3 vertPos; \n\
	in vec3 vertNormal; \n\
	\n\
	uniform uint selectedNode; \n\
	uniform vec3 cameraPos; \n\
	\n\
	out vec4 outColor; \n\
	\n\
	void main() { \n\
		vec3 unselectedColor = vec3(0.45, 0.58, 0.82); \n\
		vec3 selectedColor = vec3(0.96, 0.64, 0.12); \n\
		\n\
		vec3 nodeColor = (instanceId == int(selectedNode)) ? selectedColor : unselectedColor; \n\
		vec3 lightPos = cameraPos + vec3(1.0, 1.0, 1.0); \n\
		vec3 lightColor = vec3(1.0, 1.0, 1.0); \n\
		\n\
		float ambientStrength = 0.1; \n\
		vec3 ambient = ambientStrength * lightColor; \n\
		\n\
		vec3 normal = normalize(vertNormal); \n\
		vec3 lightDir = normalize(lightPos - vertPos); \n\
		vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor; \n\
		\n\
		vec3 viewDir = normalize(cameraPos - vertPos); \n\
		vec3 reflectDir = reflect(-lightDir, vertNormal); \n\
		\n\
		float specularStrength = 0.1; \n\
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); \n\
		vec3 specular = specularStrength * spec * lightColor; \n\
		\n\
		vec3 color = (ambient + diffuse + specular) * nodeColor; \n\
		outColor = vec4(color, 1.0); \n\
	} \n\
	";

	const std::string edgeVert =
	" \
	#version 330 core \n\
	\n\
	layout(location = 0) in vec3 position; \n\
	layout(location = 1) in uint sourceId; \n\
	layout(location = 2) in uint targetId; \n\
	layout(location = 3) in float sourceX; \n\
	layout(location = 4) in float sourceY; \n\
	layout(location = 5) in float sourceZ; \n\
	layout(location = 6) in float targetX; \n\
	layout(location = 7) in float targetY; \n\
	layout(location = 8) in float targetZ; \n\
	\n\
	uniform mat4 projection; \n\
	uniform mat4 view; \n\
	uniform mat4 model; \n\
	uniform uint selectedNode; \n\
	\n\
	flat out uint edgeId1; \n\
	flat out uint edgeId2; \n\
	\n\
	void main() \n\
	{ \n\
		edgeId1 = sourceId; \n\
		edgeId2 = targetId; \n\
		\n\
		float thickness = (edgeId1 == selectedNode || edgeId2 == selectedNode) ? 4 : 2;\n\
		\n\
		vec3 source = vec3(sourceX, sourceY, sourceZ); \n\
		vec3 target = vec3(targetX, targetY, targetZ); \n\
		\n\
		float length = sqrt(pow(source.x - target.x, 2) + pow(source.y - target.y, 2) + pow(source.z - target.z, 2)); \n\
		vec3 offset = source - vec3(0.0, 0.0, 0.0); \n\
		\n\
		vec3 rotationX = target - offset; \n\
		vec3 rotationY = normalize(cross(vec3(0.0f, 1.0f, 0.0f), rotationX)); \n\
		vec3 rotationZ = cross(rotationX, rotationY); \n\
		\n\
		rotationX = normalize(rotationX); \n\
		rotationY = normalize(rotationY); \n\
		rotationZ = normalize(rotationZ); \n\
		\n\
		mat4 rotation = mat4(vec4(rotationX, 0.0), vec4(rotationY, 0.0), vec4(rotationZ, 0.0), vec4(0.0, 0.0, 0.0, 1.0)); \n\
		mat4 translation = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(offset, 1.0)); \n\
		\n\
		vec4 newPos = translation * rotation * vec4(position.x * length, position.y * thickness, position.z * thickness, 1.0); \n\
		\n\
		gl_Position = projection * view * model * newPos; \n\
	} \n\
	";

	const std::string edgeFrag =
	" \
	#version 330 core \n\
	flat in uint edgeId1; \n\
	flat in uint edgeId2; \n\
	\n\
	\n\
	uniform uint selectedNode; \n\
	\n\
	out vec4 outColor; \
	\n\
	void main() { \n\
		vec4 unselectedColor = vec4(0.45, 0.45, 0.45, 1.0); \n\
		vec4 selectedColor = vec4(0.96, 0.64, 0.12, 1.0); \n\
		\n\
		outColor = (edgeId1 == selectedNode || edgeId2 == selectedNode) ? selectedColor : unselectedColor; \n\
	} \n\
	";

	const std::string selectVert =
	" \
	#version 330 core \n\
	\n\
	layout(location = 0) in vec3 position; \n\
	layout(location = 1) in float offsetX; \n\
	layout(location = 2) in float offsetY; \n\
	layout(location = 3) in float offsetZ; \n\
	layout(location = 4) in uint scale; \n\
	\n\
	uniform mat4 projection; \n\
	uniform mat4 view; \n\
	uniform mat4 model; \n\
	\n\
	uniform mat3 normalMatrix; \n\
	\n\
	flat out int instanceId; \n\
	out vec3 vertPos; \n\
	out vec3 vertNormal; \n\
	\n\
	void main() \n\
	{ \n\
		instanceId = gl_InstanceID; \n\
		vertPos = vec3(model * vec4(position, 1.0f)); \n\
		vertNormal = normalMatrix * normalize(position); \n\
		\n\
		vec3 offset = vec3(offsetX, offsetY, offsetZ); \n\
		vec3 newPos = position * (float(scale) * 0.5 + 0.5) + offset; \n\
		gl_Position = projection * view * model * vec4(newPos, 1); \n\
	} \n\
	";

	const std::string selectFrag =
	" \
	#version 330 core \n\
	\n\
	flat in int instanceId; \n\
	in vec3 vertPos; \n\
	in vec3 vertNormal; \n\
	\n\
	uniform vec3 cameraPos; \n\
	\n\
	out vec4 outColor; \n\
	\n\
	void main() { \n\
		float r = ((instanceId & 0x000000FF) >> 0) / 255.0f; \n\
		float g = ((instanceId & 0x0000FF00) >> 8) / 255.0f; \n\
		float b = ((instanceId & 0x00FF0000) >> 16) / 255.0f; \n\
		\n\
		outColor = vec4(r, g, b, 1.0); \n\
	} \n\
	";

	const std::string textVert = 
	" \
	#version 330 core \n\
	\n\
	layout(location = 0) in vec4 vertex; \n\
	\n\
	uniform mat4 projection; \n\
	\n\
	out vec2 textureCoord; \n\
	\n\
	void main() \n\
	{ \n\
		textureCoord = vertex.zw; \n\
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0); \n\
	} \n\
	";

	const std::string textFrag = 
	" \
	#version 330 core \n\
	in vec2 textureCoord; \n\
	\n\
	uniform sampler2D text; \n\
	\n\
	out vec4 outColor; \n\
	\n\
	void main() \n\
	{ \n\
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoord).r); \n\
		outColor = vec4(0.25, 0.25, 0.25, 1.0) * sampled; \n\
	} \n\
	";

}

#endif
