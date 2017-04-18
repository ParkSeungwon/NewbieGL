#version 130
uniform mat4 KeyBindMatrix;
in vec3 a_pos;
in vec3 a_color;
in vec3 norm;
out vec3 color;
out vec4 N;
out vec3 pos;
mat4 trans = mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0.5,0.2,0.2,1);
void main() {
	gl_Position = KeyBindMatrix * vec4(a_pos, 1.0);
	N = KeyBindMatrix * vec4(norm, 1.0);
	color = a_color;
	pos = a_pos;
}

