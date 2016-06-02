#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 color[];
in vec2 speed[];

out vec4 out_color;

uniform float width;
uniform float height;

void main() {
	out_color = color[0];

	gl_Position = gl_in[0].gl_Position + vec4(height / 2.0f, -width / 2.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-height / 2.0f, -width / 2.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-height / 2.0f, width / 2.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(height / 2.0f, width / 2.0f, 0.0f, 0.0f);
	EmitVertex();
}
