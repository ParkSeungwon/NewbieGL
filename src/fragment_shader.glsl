#version 130
in vec3 color;
in vec4 N;
in vec3 pos;
out vec4 f_color;

vec3 ambient = vec3(0.1,0.1,0.1);
vec3 diffuse = vec3(0.2,0.2,0.2);
vec3 specular = vec3(0.5,0.5,0.5);
vec3 light = vec3(0, 0.9, 0);
vec3 view = vec3(0,0,2);

void main() {
	vec3 NN = normalize(N.xyz);
	vec3 L = normalize(light - pos);
	vec3 E = -normalize(pos);
	vec3 H = normalize(L+E);
	float kd = max(dot(L, NN), 0);
	float ks = pow(max(dot(NN, H), 0), 5);
	vec3 diffuse_p = kd * diffuse;
	vec3 spec_p = max(pow(max(dot(NN, H), 0), 5) * specular, 0);

	f_color = vec4(color * (ambient + diffuse_p + spec_p), 1.0);
}

