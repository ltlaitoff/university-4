#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*!
 * OpenGL Shader program wrapper for work with GLSL shader files
 */
class Shader {
public:
	/*!
	* Create Shader program from two shader-files
	* 
	* @param vertexPath - Path to vertex shader file in file system
	* @param fragmentPath - Path to vertex shader file in file system
	*/
	Shader(const char* vertexPath, const char* fragmentPath);

	/*!
	 * Use current shader
	 */
	void use();

	/*!
	 * Utils for set `uniform` variables in shaders
	 */
	void setVec2(const std::string& name, float x, float y) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;
	void setInt(const std::string& name, int number) const;
private:
	GLuint ID;
	
	enum Error {
		PROGRAM,
		VERTEX,
		FRAGMENT
	};

	const std::string readShaderFile(std::string path);
	void checkOnErrors(GLuint shader, Shader::Error type);
};
#endif