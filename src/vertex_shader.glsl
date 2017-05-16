#version 130
uniform mat4 KeyBindMatrix;
in vec3 a_pos;
in vec3 a_color;
in vec3 norm;
in vec3 a_uv;

vec2 uv = a_uv.xy;

out vec3 color;
out vec4 NN;
out vec3 pos;
out vec2 f_uv;
void main() {
	gl_Position = KeyBindMatrix * vec4(a_pos, 1.0);
	NN = KeyBindMatrix * vec4(norm, 0.0f);
	color = a_color;
	pos = a_pos;
	f_uv = uv;
}

