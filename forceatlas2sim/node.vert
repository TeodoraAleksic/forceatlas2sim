#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in float offsetX;
layout(location = 2) in float offsetY;
layout(location = 3) in float offsetZ;
layout(location = 4) in uint scale;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main ()
{
	vec3 offset = vec3(offsetX, offsetY, offsetZ);
	vec3 newPos = position * (float(scale) * 0.5) + offset;
	gl_Position = projection * view * model * vec4(newPos, 1);
}
