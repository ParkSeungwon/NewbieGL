#version 330

uniform float Scale;
uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectViewMatrix;

in vec3 a_pos;
out vec4 v_color;

void main()
{
	gl_Position = gl_ModelViewMatrix * gl_ProjectViewMatrix * a_pos;
	v_color = vec4(a_pos, 1.0);
}
