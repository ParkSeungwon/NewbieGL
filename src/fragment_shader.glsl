#version 130
uniform mat4 LIGHT;
uniform mat4 KeyBindMatrix;
uniform samplerCube a_uv;
in vec3 color;
in vec4 NN;
in vec3 pos;
in vec3 f_uv;
out vec4 f_color;

vec3 ambient = LIGHT[0].xyz;
vec3 diffuse = LIGHT[1].xyz;
vec3 specular = LIGHT[2].xyz;
vec3 light_pos = LIGHT[3].xyz;
vec3 view = vec3(0,0,2);

void main() {
	vec4 lp = vec4(light_pos, 1);
	lp = KeyBindMatrix * lp;
	vec3 N = normalize(NN.xyz);
	vec3 L = normalize(lp.xyz - pos);
	vec3 V = normalize(view);
	vec3 R = -V + 2 * dot(N, V) * N;

	vec4 texture = textureCube(a_uv, f_uv);
	f_color = vec4(texture.xyz * (0.1 * ambient + 0.8 * diffuse * dot(L, N) + specular * pow(dot(V, R), 25)), 1.0);
}

