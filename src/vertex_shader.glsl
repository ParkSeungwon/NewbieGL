#version 130
uniform mat4 KeyBindMatrix;
in vec3 vertexes_;
in vec3 colors_;
in vec3 normals_;

out vec3 color;
out vec4 NN;
out vec3 pos;
out vec3 f_uv;
void main() {
	gl_Position = KeyBindMatrix * vec4(vertexes_, 1.0);
	NN = KeyBindMatrix * vec4(normals_, 0.0f);
	color = colors_;
	pos = vertexes_;
	f_uv = reflect(vertexes_, NN.xyz);
}

