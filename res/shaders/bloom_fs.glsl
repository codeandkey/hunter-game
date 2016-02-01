#version 330

in vec2 p_texcoord;

uniform sampler2D tds_texture;
uniform vec4 tds_color;

out vec4 color;

void main(void) {
	vec4 inp_color = texture2D(tds_texture, p_texcoord) * tds_color;

	/* More gaussian weight black magic */
	if (dot(inp_color.xyz, vec3(0.2126, 0.7152, 0.0722)) > 0.75f) {
		color = inp_color;
	} else {
		discard;
	}
}
