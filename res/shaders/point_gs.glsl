#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 p_texcoord[];
out vec2 out_normal;

void main() {
	if (dot(p_texcoord[0], gl_in[0].gl_Position.xy) < 0.0) {
		return;
	}

	float projection_distance = 1000.0f;

	gl_Position = gl_in[0].gl_Position;
	out_normal = p_texcoord[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	out_normal = p_texcoord[1];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position * vec4(projection_distance, projection_distance, 1.0f, 1.0f);
	out_normal = p_texcoord[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position * vec4(projection_distance, projection_distance, 1.0f, 1.0f);
	out_normal = p_texcoord[1];
	EmitVertex();

	EndPrimitive();
}
