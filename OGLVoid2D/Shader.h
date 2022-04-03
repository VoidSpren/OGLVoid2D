#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
	uint32_t id;
public:
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::ifstream vertexFile(vertexPath), fragmentFile(fragmentPath);
		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		std::string vertexCode = vertexStream.str(), fragmentCode = fragmentStream.str();

		programLinking(vertexCode, fragmentCode);
		use();
	}

	void use() { glUseProgram(id); }

	void setBool(const std::string& name, bool val) {
		glUniform1i(
			glGetUniformLocation(id, name.c_str()),
			(int)val
		);
	}
	void setInt(const std::string& name, int val) {
		glUniform1i(
			glGetUniformLocation(id, name.c_str()),
			val
		);
	}
	void setUInt(const std::string& name, uint32_t val) {
		glUniform1ui(
			glGetUniformLocation(id, name.c_str()),
			val
		);
	}
	void setFloat(const std::string& name, float val) {
		glUniform1f(
			glGetUniformLocation(id, name.c_str()),
			val
		);
	}
	void setVec2(const std::string& name, float x, float y) {
		glUniform2f(
			glGetUniformLocation(id, name.c_str()),
			x, y
		);
	}
	void setVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(
			glGetUniformLocation(id, name.c_str()),
			x, y, z
		);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(
			glGetUniformLocation(id, name.c_str()),
			x, y, z, w
		);
	}

private:
	uint32_t shaderCompilation(const char* shaderSource, GLenum type) {
		uint32_t shader;
		shader = glCreateShader(type);

		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		shaderErrorPrint(shader, type);

		return shader;
	}

	bool shaderErrorPrint(uint32_t shader, GLenum type) {
		int success;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infolog[512];
			glGetShaderInfoLog(shader, 512, NULL, infolog);
			std::cout << "ERROR::SHADER::" << type << "COMPILATION_FAILED\n" << infolog << std::endl;
		}

		return success;
	}

	void programLinking(std::string& vertexCode, std::string& fragmentCode) {
		id = glCreateProgram();

		/*compile shaders*/
		uint32_t vertex = shaderCompilation(vertexCode.c_str(), GL_VERTEX_SHADER);
		uint32_t fragment = shaderCompilation(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
		/*attaching the vertex and fragment shader to the program*/
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		/*linking program to opengl*/
		glLinkProgram(id);
		/*check for errors*/
		LINKLOG();

		glDeleteShader(vertex); glDeleteShader(fragment);
	}

	void LINKLOG() {
		int success;

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success) {
			char infolog[512];
			glGetProgramInfoLog(id, 512, NULL, infolog);
			std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infolog << std::endl;
		}
	}
};
