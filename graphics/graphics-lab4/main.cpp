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
	glEnable(GL_DEPTH_TEST);

	// First 3 is pos, second 3 is color in RGB
	float vertices[] = {
		// Base, shared for both
		-0.5f, 0.5f, 0.3f,		1.0f, 1.0f, 1.0f, // A aka top-left
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 0.0f, // B aka bottom-right
		
		// First
		0.7f, 0.7f, 0.5f,		1.0f, 0.0f, 0.0f, // C aka top-right  
		0.5f, 0.0f, 0.5f,		1.0f, 0.6f, 0.0f, // D new for 3d

		// Second
		-0.7f, -0.7f, 0.3f,		0.6f, 1.0f, 0.0f, // K aka bottom left
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f, // G new for 3d
	};

	unsigned int indices[] = {
		// First
		0, 1, 2,
		0, 1, 3,
		1, 2, 3,
		0, 2, 3,

		// Second
		0, 1, 4,
		0, 1, 5,
		1, 4, 5,
		0, 4, 5,
	};

	Shader greenShader("base.vert", "base.frag");
	Shader skyShader("base.vert", "base.frag");

	GLuint VAO1 = initVAO(vertices, sizeof(vertices), indices, sizeof(indices));
	GLuint VAO2 = initVAO(vertices, sizeof(vertices), indices, sizeof(indices));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.945f, 0.961f, 0.976f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		skyShader.use();
		skyShader.setVec2("u_resolution", WIDTH, HEIGHT);
		skyShader.setMat4("model", model);
		skyShader.setMat4("view", view);
		skyShader.setMat4("projection", projection);
		glBindVertexArray(VAO2);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)0);

		greenShader.use();
		greenShader.setVec2("u_resolution", WIDTH, HEIGHT);
		greenShader.setMat4("model", model);
		greenShader.setMat4("view", view);
		greenShader.setMat4("projection", projection);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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