#version 330

in vec2 p_texcoord;
in vec2 p_texcoord_blur[14];

uniform sampler2D tds_texture;
uniform vec4 tds_color;

out vec4 color;

void main(void) {
	color = vec4(0.0f);

	/* Apply a Gaussian convolution */

	color += texture2D(tds_texture, p_texcoord_blur[0]) * 0.0044299121055113265;
	color += texture2D(tds_texture, p_texcoord_blur[1]) * 0.00895781211794;
	color += texture2D(tds_texture, p_texcoord_blur[2]) * 0.0215963866053;
	color += texture2D(tds_texture, p_texcoord_blur[3]) * 0.0443683338718;
	color += texture2D(tds_texture, p_texcoord_blur[4]) * 0.0776744219933;
	color += texture2D(tds_texture, p_texcoord_blur[5]) * 0.115876621105;
	color += texture2D(tds_texture, p_texcoord_blur[6]) * 0.147308056121;
	color += texture2D(tds_texture, p_texcoord) * 0.159576912161;
	color += texture2D(tds_texture, p_texcoord_blur[7]) * 0.147308056121;
	color += texture2D(tds_texture, p_texcoord_blur[8]) * 0.115876621105;
	color += texture2D(tds_texture, p_texcoord_blur[9]) * 0.0776744219933;
	color += texture2D(tds_texture, p_texcoord_blur[10]) * 0.0443683338718;
	color += texture2D(tds_texture, p_texcoord_blur[11]) * 0.0215963866053;
	color += texture2D(tds_texture, p_texcoord_blur[12]) * 0.00895781211794;
	color += texture2D(tds_texture, p_texcoord_blur[13]) * 0.0044299121055113265;
}
