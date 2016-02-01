#version 330

in vec3 v_position;
in vec2 v_texcoord;

out vec2 p_texcoord;
out vec2 p_texcoord_blur[14];

uniform mat4 tds_transform;

void main(void) {
	gl_Position = tds_transform * vec4(v_position, 1.0);
	p_texcoord = v_texcoord;

	/* Precompute the texture coordinates for the blur; this greatly increases shader performance */

	p_texcoord_blur[0] = p_texcoord + vec2(-0.028f, 0.0f);
	p_texcoord_blur[1] = p_texcoord + vec2(-0.024f, 0.0f);
	p_texcoord_blur[2] = p_texcoord + vec2(-0.020f, 0.0f);
	p_texcoord_blur[3] = p_texcoord + vec2(-0.016f, 0.0f);
	p_texcoord_blur[4] = p_texcoord + vec2(-0.012f, 0.0f);
	p_texcoord_blur[5] = p_texcoord + vec2(-0.008f, 0.0f);
	p_texcoord_blur[6] = p_texcoord + vec2(-0.004f, 0.0f);
	p_texcoord_blur[7] = p_texcoord + vec2(0.004f, 0.0f);
	p_texcoord_blur[8] = p_texcoord + vec2(0.008f, 0.0f);
	p_texcoord_blur[9] = p_texcoord + vec2(0.012f, 0.0f);
	p_texcoord_blur[10] = p_texcoord + vec2(0.016f, 0.0f);
	p_texcoord_blur[11] = p_texcoord + vec2(0.020f, 0.0f);
	p_texcoord_blur[12] = p_texcoord + vec2(0.024f, 0.0f);
	p_texcoord_blur[13] = p_texcoord + vec2(0.028f, 0.0f);
}
