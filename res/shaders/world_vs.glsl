#version 330

in vec3 v_position;
in vec2 v_texcoord;

out vec2 p_texcoord;

uniform mat4 tds_transform;

void main(void) {
	gl_Position = tds_transform * vec4(v_position, 1.0);
	p_texcoord = v_texcoord;
}
