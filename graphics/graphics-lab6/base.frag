#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main() {
	// gl_FragColor = vec4(outColor, 1.0f);
	FragColor = texture(ourTexture, TexCoord);
}