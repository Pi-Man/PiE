#version 330 core

in vec2 uv;
in vec3 Normal;

out vec4 col;

uniform float time;

void main() {

	float blur = 0.0;
	float r = (abs(dFdx(uv.x)) + abs(dFdy(uv.y))) * 0.5f + blur;
	float scale = 1 + r * 2;

	vec2 uv_scaled = (uv - 0.5) * scale + 0.5f;

	float val = (uv_scaled.x*uv_scaled.x - uv_scaled.y) * (float(gl_FrontFacing) * 2 - 1);
	val = smoothstep(-r, r, val);

	//gl_FragDepth = gl_FragCoord.z + (1 - val) * 0.01f;

	col = vec4(Normal, val);
	//col = vec4((float(gl_FrontFacing) * 2 - 1), -(float(gl_FrontFacing) * 2 - 1), 0, 1);
}