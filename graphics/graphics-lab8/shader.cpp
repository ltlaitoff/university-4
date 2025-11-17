#include "shader.hpp"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	const std::string vertexCodeRaw = Shader::readShaderFile(vertexPath);
	const std::string fragmentCodeRaw = Shader::readShaderFile(fragmentPath);

	const char* vertexCode = vertexCodeRaw.c_str();
	const char* fragmentCode = fragmentCodeRaw.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	Shader::checkOnErrors(vertexShader, Shader::Error::VERTEX);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	Shader::checkOnErrors(fragmentShader, Shader::Error::FRAGMENT);

	Shader::ID = glCreateProgram();

	glAttachShader(Shader::ID, vertexShader);
	glAttachShader(Shader::ID, fragmentShader);
	glLinkProgram(Shader::ID);
	Shader::checkOnErrors(Shader::ID, Shader::Error::PROGRAM);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(Shader::ID);
}

// TODO: Add GLM version?

void Shader::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(Shader::ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 vector) const {
	glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(Shader::ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setInt(const std::string& name, int number) const {
	glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), number);
}

void Shader::setFloat(const std::string& name, float number) const {
	glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), number);
}

const std::string Shader::readShaderFile(std::string path) {
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		code = stream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return code;
}

void Shader::checkOnErrors(GLuint shader, Shader::Error type) {
	GLint success;
	GLchar infoLog[1024];

	if (type == Shader::Error::PROGRAM) {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);

			std::cout << "ERROR::SHADER::LINK_FAILED\n" <<
				infoLog << std::endl;
		}

		return;
	}

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		std::cout << "ERROR::SHADER::SHADER_COMPILATION_ERROR in type:" << type << "\n" <<
			infoLog << std::endl;
	}
}
