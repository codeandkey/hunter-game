#version 330

layout (points) in;
layout (points, max_vertices=1) out;

in vec2 position;
in vec4 color;
in vec2 speed;

void main(void) {
	gl_Position = vec4(position, 0.0f, 1.0f) + vec4(speed.x, speed.y, 0.0f, 0.0f);
	EmitVertex();
}
