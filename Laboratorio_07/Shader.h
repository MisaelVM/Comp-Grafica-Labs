#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader {
private:
	unsigned int shaderProgramID;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void use() const;
	void disuse() const;

	// Uniforms
	void setUniform1i(const std::string& name, int v0);

	void setUniform1f(const std::string& name, float v0);
	void setUniform2f(const std::string& name, float v0, float v1);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void setUniformMatrix4f(const std::string& name, glm::mat4 v0);

private:
	std::string parseShader(const std::string& filePath);
	unsigned int checkProgramErrors(unsigned int programID, unsigned int programName, const std::string& msg);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(unsigned int vertexShader, unsigned int fragmentShader);
	int getUniformLocation(const std::string& name);
};

#endif
