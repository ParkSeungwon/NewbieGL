#version 130
uniform mat4 LIGHT;
uniform mat4 KeyBindMatrix;
uniform mat4 nTex;
uniform sampler2D TEXTURE0;
uniform sampler2D TEXTURE1;
in vec3 color;
in vec4 NN;
in vec3 pos;
out vec4 f_color;

vec3 ambient = LIGHT[0].xyz;
vec3 diffuse = LIGHT[1].xyz;
vec3 specular = LIGHT[2].xyz;
vec3 light_pos = LIGHT[3].xyz;
vec3 view = vec3(0,0,3);

void main() {
	vec4 lp = vec4(light_pos, 1);
	lp = KeyBindMatrix * lp;
	vec3 N = normalize(sin(NN.xyz*30));
	vec3 L = normalize(lp.xyz - pos);
	vec3 V = normalize(view);
	vec3 R = -V + 2 * dot(N, V) * N;

	vec3 shade = 0.1*ambient + 0.8*diffuse*dot(L, N) + specular*pow(dot(V, R), 25);
	
	vec4 tt;
	if(nTex[0].x == 0) tt = texture(TEXTURE0, color.xy);
	else if(nTex[0].x == 1) tt = texture(TEXTURE1, color.xy);
	else tt = vec4(color, 1.0);
	
	f_color = vec4(tt.rgb * shade, tt.a);
}

