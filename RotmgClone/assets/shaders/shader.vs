#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 pvm;

void main() {
	vec4 pos = pvm * vec4(aPos.xyz, 1.0f);
	pos.y -= aPos.z;
	pos.z = -pos.z;
	
	gl_Position = pos;
	
	texCoord = aTexCoord;
}
