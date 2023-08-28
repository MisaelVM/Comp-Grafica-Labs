#pragma once

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/VertexArray.h>
#include <GL/ElementBuffer.h>
#include <GL/Shader.h>

#include <queue>
#include <vector>

class RubikAnimation {
private:
	std::vector<glm::vec3>& positions;
	std::vector<glm::mat4>& transforms;
	std::vector<glm::mat4> initialTransforms;
	std::vector<int> interestingIndices;
	std::string movement;
	glm::vec3 rotationAxis;
	float rotationAngle;
	float duration;
	float timeSoFar;

public:
	RubikAnimation(std::vector<glm::vec3>& pos, std::vector<glm::mat4>& trans, std::string movement, float duration = 1.0f);

	bool completed = false;
	bool started = false;

	void start();
	void update(float deltaTime);
};

class RubiksCube {
private:
	std::vector<glm::vec3> cubPositions;
	std::vector<glm::mat4> cubTransforms;

	std::vector<std::string> scrambleCache;
	std::queue<RubikAnimation*> movementsQueue;

	void addMovement(RubikAnimation* rubikAnim);

	// Estructura logica

public:
	RubiksCube();

	void addMovement(std::string movement, float duration = 1.0f);
	void scramble(int maxMovements = 35);

	void update(float deltaTime);
	void draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const;
	void solve();
};

#endif
