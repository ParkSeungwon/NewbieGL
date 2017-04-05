#version 130
uniform mat4 KeyBindMatrix;
in vec3 a_pos;

void main() {
	gl_Position = KeyBindMatrix * vec4(a_pos, 1.0);
}

