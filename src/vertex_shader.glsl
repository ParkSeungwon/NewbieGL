#version 130
uniform mat4 KeyBindMatrix;
in vec3 a_pos;
in vec3 a_color;
out vec3 color;
mat4 trans = mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0.5,0.2,0.2,1);
void main() {
	gl_Position = KeyBindMatrix * trans * vec4(a_pos, 1.0);
	color = a_color;
}

