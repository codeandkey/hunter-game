#version 330

in vec2 p_texcoord;

uniform sampler2D tds_texture;
uniform vec4 tds_color;

out vec4 color;

void main(void) {
	color = texture2D(tds_texture, p_texcoord);
}
