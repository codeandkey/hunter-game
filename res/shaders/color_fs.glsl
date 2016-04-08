#version 330

in vec2 p_texcoord;

uniform vec4 tds_color;
uniform sampler2D tex;

out vec4 color;

void main(void) {
	color = tds_color;
}
