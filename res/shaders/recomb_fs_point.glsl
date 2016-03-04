#version 330

in vec2 p_texcoord;

uniform sampler2D tds_texture;
uniform vec4 tds_color;

out vec4 color;

void main(void) {
	float dist = sqrt(pow(p_texcoord.x - 0.5f, 2) + pow(p_texcoord.y - 0.5f, 2)) * 3.0f;

	color = texture2D(tds_texture, p_texcoord) / dist;
	//color = vec4(p_texcoord.x, 0.0f, p_texcoord.y, 1.0f);
}
