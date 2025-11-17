#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 aColor;

out vec4 VertColor;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 fragmentPos = vec3(model * vec4(aPos, 1.0));

	VertColor = vec4(aColor, 1.0f);
}
