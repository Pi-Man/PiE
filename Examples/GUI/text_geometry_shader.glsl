#version 330 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

in vec2 uv[];

out vec2 uv_frag;

uniform float time;

void main() {

	float canBlur = 1 - float(uv[0] == vec2(0.5, 0.5) && uv[1] == vec2(0.5, 0.5) && uv[2] == vec2(0.5, 0.5));
	
	float blur = 0 * canBlur;
	float scale = 1.1 + blur * 2;

	vec4 start = gl_in[0].gl_Position;
	vec4 end = gl_in[2].gl_Position;
	vec4 mid = (end + start) * 0.5;

	gl_Position = (gl_in[0].gl_Position - mid) * scale + mid;
	uv_frag = uv[0];
	EmitVertex();
	gl_Position = (gl_in[1].gl_Position - mid) * scale + mid;
	uv_frag = uv[1];
	EmitVertex();
	gl_Position = (gl_in[2].gl_Position - mid) * scale + mid;
	uv_frag = uv[2];
	EmitVertex();
	EndPrimitive();
}