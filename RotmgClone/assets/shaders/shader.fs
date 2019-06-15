#version 450 core

in vec2 texCoord;

uniform sampler2D tex;

out vec4 color_out;

void main() {
	color_out = texture(tex, texCoord);
}