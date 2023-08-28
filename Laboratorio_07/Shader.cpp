#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	std::string vertexShaderSource = parseShader(vertexShaderPath);
	std::string fragmentShaderSource = parseShader(fragmentShaderPath);

	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	shaderProgramID = createShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(shaderProgramID);
}

void Shader::use() const {
	glUseProgram(shaderProgramID);
}

void Shader::disuse() const {
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int v0) {
	glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1f(const std::string& name, float v0) {
	glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform2f(const std::string& name, float v0, float v1) {
	glUniform2f(getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMatrix4f(const std::string& name, glm::mat4 v0) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0));
}

std::string Shader::parseShader(const std::string& filePath) {
	std::string shaderSource;
	std::ifstream stream(filePath);
	std::stringstream ss;

	if (!stream.is_open()) {
		std::cout << "Failed to read Shader file\n";
		return "\0";
	}

	ss << stream.rdbuf();
	stream.close();

	return ss.str();
}

unsigned int Shader::checkProgramErrors(unsigned int programID, unsigned int programName, const std::string& msg) {
	int success;
	int length;
	char* infoLog;

	if (programName == GL_COMPILE_STATUS) {
		glGetShaderiv(programID, programName, &success);
		if (!success) {
			glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &length);
			infoLog = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(programID, length, &length, infoLog);
			std::cout << "Failed to compile " << msg << "!\n" << infoLog << "\n";
			glDeleteShader(programID);
			return 0;
		}
	}
	else {
		glGetProgramiv(programID, programName, &success);
		if (!success) {
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
			infoLog = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(programID, length, &length, infoLog);
			std::cout << "Failed to link " << msg << "!\n" << infoLog << "\n";
			glDeleteProgram(programID);
			return 0;
		}
	}

	return programID;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	return checkProgramErrors(id, GL_COMPILE_STATUS, (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader"));
}

unsigned int Shader::createShader(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int id = glCreateProgram();

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	id = checkProgramErrors(id, GL_LINK_STATUS, "shader program");

	glValidateProgram(id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return id;
}

int Shader::getUniformLocation(const std::string& name)
{
	if (uniformLocationCache.count(name))
		return uniformLocationCache[name];

	int location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' does not exist!\n";
	else
		uniformLocationCache[name] = location;
	return location;
}
