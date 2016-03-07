#version 330

in vec2 p_texcoord;

uniform sampler2D tds_texture;
uniform vec4 tds_color;

out vec4 color;

void main(void) {
	vec4 texcolor = texture2D(tds_texture, p_texcoord);
	float dist = min(sqrt(pow(p_texcoord.x - 0.5f, 2) + pow(p_texcoord.y - 0.5f, 2)) * 2.0f, 1.0f);

	color = texcolor * (1.0f - dist);
}
