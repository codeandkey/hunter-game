#version 330

// overlay blending because it's better
// we dont want to illuminate dark surfaces but we do want to add to light surfaces
// so, multiplicative will fix the former
// we can use an extra offset to help the latter

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

float pegtop(float src, float dst) {
	return (1 - 2 * src) * pow(dst, 2) + 2 * src * dst;
}

void main(void) {
	vec4 dst = texture2D(tds_texture, p_texcoord) * tds_color;
	vec4 src = texture2D(tds_texture2, p_texcoord);

	color = dst * (src * 3.0f); // allow src to overflow (hdr?)
	color.a = 1;
	return;

	/*
	color.r = overlay(src.r, dst.r);
	color.g = overlay(src.g, dst.g);
	color.b = overlay(src.b, dst.b);
	*/

	color.r = pegtop(src.r, dst.r);
	color.g = pegtop(src.g, dst.g);
	color.b = pegtop(src.b, dst.b);

	color.a = tds_color.a;
}
