#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in float sourceX;
layout(location = 2) in float sourceY;
layout(location = 3) in float sourceZ;
layout(location = 4) in float targetX;
layout(location = 5) in float targetY;
layout(location = 6) in float targetZ;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vec3 source = vec3(sourceX, sourceY, sourceZ);
	vec3 target = vec3(targetX, targetY, targetZ);

	float scale = sqrt(pow(source.x - target.x, 2) + pow(source.y - target.y, 2) + pow(source.z - target.z, 2));;
	vec3 offset = source - vec3(0.0, 0.0, 0.0);

	vec3 rotationX = target - offset;
	vec3 rotationY = normalize(cross(vec3(0.0f, 1.0f, 0.0f), rotationX));
	vec3 rotationZ = cross(rotationX, rotationY);

	rotationX = normalize(rotationX);
	rotationY = normalize(rotationY);
	rotationZ = normalize(rotationZ);

	mat4 rotation = mat4(vec4(rotationX, 0.0), vec4(rotationY, 0.0), vec4(rotationZ, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
	mat4 translation = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(offset, 1.0));

	vec4 newPos = translation * rotation * vec4(position.x * scale, position.y, position.z, 1.0);

	gl_Position = projection * view * model * newPos;
}
