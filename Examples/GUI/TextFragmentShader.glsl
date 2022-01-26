#version 330 core

in vec3 Normal;

out vec4 col;

uniform vec2 control_points[1024];
uniform int indices[1024];

uniform ivec2 viewport;

uniform float time;

const float blur = 2;

const float pi = atan(0, -1);

#define ITERATIONS 4

vec2 ternary(vec2 condition, vec2 true_statement, vec2 false_statement) {
	return condition * true_statement + (1 - condition) * false_statement;
}

void main() {

	int char_code = int(Normal.z);
	vec2 uv = Normal.xy;

	int start_index = indices[char_code];
	int end_index = indices[char_code + 1];

	float r = (dFdx(uv.x) + dFdy(uv.y)) * 0.5f * blur;
	float rinv = 1.0f/r;

	float accum = 0;

	float test[3] = {0, 0, 0};

	for (int i = 0; i < ITERATIONS; i++) {

		float angle = pi / ITERATIONS * i;

		float cosa = cos(angle), sina = sin(angle);

		mat2 T = {{cosa, sina}, {-sina, cosa}};

		float val = 0;

		for (int index = start_index; index < end_index; index += 3) {
			int i2 = (index - start_index) / 3;

			vec2 p1 = T * (control_points[index + 0] - uv);
			vec2 p2 = T * (control_points[index + 1] - uv);
			vec2 p3 = T * (control_points[index + 2] - uv);

			vec2 p2_1 = p2 - p1;
			vec2 p3_2 = p3 - p2;

			float crossVal = p2_1.x * p3_2.y - p2_1.y * p3_2.x;

			float tl = p3.y / (2 * p3_2.y);
			float xl = 2 * -p3_2.x * tl + p3.x;
			float t2l = (xl*rinv + 1) * 0.5;
			float lineVal = -float(tl > 0 && tl < 1) * sign(p3_2.y) * clamp(t2l, 0, 1);

//			float det = p3_2.y * p3_2.y - p3.y * (p1.y - p2.y + p3_2.y);
//
//			float sqrtdet = sqrt(det);
//			vec2 t = vec2(p3_2.y + sqrtdet, p3_2.y - sqrtdet) / (p1.y - 2*p2.y + p3.y);
//			vec2 omt = 1-t;
//			vec2 x = t*t*p1.x + 2*t*omt*p2.x + omt*omt*p3.x;
//			vec2 t2 = (x*rinv + 1) * 0.5;
//			vec2 ta = vec2(t.x, t2.x);
//			vec2 tb = vec2(t.y, t2.y);
//			vec2 mint = ta.y < tb.y ? ta : tb;
//			vec2 maxt = ta.y > tb.y ? ta : tb;
//			mint.y = clamp(mint.y, 0, 1);
//			maxt.y = clamp(maxt.y, 0, 1);
//			float conicVal = float(mint.x >= 0 && mint.x < 1) * crossVal * mint.y - float(maxt.x >= 0 && maxt.x < 1) * crossVal * maxt.y;
//
//			val += mix(lineVal, mix(conicVal, 0, float(det > 0)), float(abs(crossVal) > 1));

			val += lineVal;


//			if (abs(crossVal) < 0.0001) {
//				float t = p3.y / (2 * p3_2.y);
//				float x = 2 * -p3_2.x * t + p3.x;
//				float t2 = (x*rinv + 1) * 0.5;
//				val -= float(t > 0 && t < 1) * sign(p3_2.y) * clamp(t2, 0, 1);
//			}
//			else {
//				float det = p3_2.y * p3_2.y - p3.y * (p1.y - p2.y + p3_2.y);
//				if (det >= 0) {
//					float sqrtdet = sqrt(det);
//					vec2 t = vec2(p3_2.y + sqrtdet, p3_2.y - sqrtdet) / (p1.y - 2*p2.y + p3.y);
//					vec2 omt = 1-t;
//					vec2 x = t*t*p1.x + 2*t*omt*p2.x + omt*omt*p3.x;
//					vec2 t2 = (x*rinv + 1) * 0.5;
//					vec2 ta = vec2(t.x, t2.x);
//					vec2 tb = vec2(t.y, t2.y);
//					vec2 mint = ta.y < tb.y ? ta : tb;
//					vec2 maxt = ta.y > tb.y ? ta : tb;
//					mint.y = clamp(mint.y, 0, 1);
//					maxt.y = clamp(maxt.y, 0, 1);
//					val += float(mint.x >= 0 && mint.x < 1) * crossVal * mint.y;
//					val -= float(maxt.x >= 0 && maxt.x < 1) * crossVal * maxt.y;
//				}
//			}
		}
		val += float(val < 0);
		accum += val;
	}
	accum /= ITERATIONS;

	col = vec4(accum, -accum, 0, 1);
}