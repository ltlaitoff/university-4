#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint initVAO(float vertices[], size_t verticesSize, unsigned int indices[], size_t indicesSize);

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW failed to start" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	float vertices[] = {
		0.7f, 0.7f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.7f, -0.7f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, // top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	Shader greenShader("transform.vert", "green.frag");
	Shader skyShader("transform.vert", "sky.frag");

	GLuint VAO1 = initVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	GLuint VAO2 = initVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.945f, 0.961f, 0.976f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::translate(trans, glm::vec3(0.5f, 0.2f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime() * 0.9f, glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime() * 0.6f, glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.75f, 0.2f));

		greenShader.use();
		greenShader.setVec2("u_resolution", WIDTH, HEIGHT);
		greenShader.setMat4("transform", trans);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

		skyShader.use();
		skyShader.setVec2("u_resolution", WIDTH, HEIGHT);
		skyShader.setMat4("transform", trans);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

/*!
 * Create Vertex Array Object from all vertices and indices
 */
GLuint initVAO(float vertices[], size_t verticesSize, unsigned int indices[], size_t indicesSize) {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

/*!
 * Process interaction with user
 */
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

/*!
 * Auto update OpenGL viewport on resize
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}