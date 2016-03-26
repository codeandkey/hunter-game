#version 330

// overlay blending because it's better

in vec2 p_texcoord;

uniform sampler2D tds_texture;
uniform sampler2D tds_texture2;
uniform vec4 tds_color;

out vec4 color;

float overlay(float b, float a) {
	if (a < 0.5f) {
		return 2 * a * b;
	} else {
		return 1.0f - 2.0f * (1.0f - a) * (1.0f - b);
	}
}

void main(void) {
	vec4 src = texture2D(tds_texture, p_texcoord) * tds_color;
	vec4 dst = texture2D(tds_texture2, p_texcoord);

	color.r = overlay(dst.r, src.r);
	color.g = overlay(dst.g, src.g);
	color.b = overlay(dst.b, src.b);

	color.a = tds_color.a;
}
