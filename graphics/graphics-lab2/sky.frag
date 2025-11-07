#version 330 core
uniform vec2 u_resolution;

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution.xy;

	vec3 color1 = vec3(0.22, 0.741, 0.973);
	vec3 color2 = vec3(1.0, 1.0, 1.0);

	float mixValue = distance(st,vec2(0,1));
	vec3 color = mix(color1,color2,mixValue);

	gl_FragColor = vec4(color, mixValue);
}