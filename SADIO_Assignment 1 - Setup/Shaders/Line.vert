#version 330 core

layout (location = 0) in vec3 aPos;

uniform float x;
uniform float y;
uniform float z;

uniform mat4 projection;
uniform mat4 view;

void main(){
	vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z + z);
	gl_Position = projection * view * vec4(newPos, 1.0f);
}