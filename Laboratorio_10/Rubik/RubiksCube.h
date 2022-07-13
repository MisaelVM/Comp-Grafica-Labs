#pragma once

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "GL/VertexArray.h"
#include "GL/VertexBuffer.h"
#include "GL/ElementBuffer.h"
#include "GL/Shader.h"
#include "GL/Texture.h"

#include "Rubik/Solver/rubik_cube.hpp"
#include "Rubik/Solver/rubik_cube_solver.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <queue>
#include <cmath>
#include <cstring>
#include <numeric>
#include <vector>

struct Cube {
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Texture* faceTextures[6];

	Cube();
	void draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const;
};

class RubikAnimation {
private:
	std::vector<Cube>& cubes;
	std::vector<Cube> initialCubes;
	std::vector<int> interestingIndices;
	std::string move;
	glm::vec3 rotationAxis;
	float rotationAngle;
	float duration;
	float timeSoFar;

public:
	RubikAnimation(std::vector<Cube>& cubs, std::string move, float duration = 1.0f);

	bool completed = false;
	bool started = false;

	void start();
	void update(float deltaTime);
};

class RubiksCube {
private:
	rb::RubikCube rb;

	Texture* textures[6][9];
	std::vector<Cube> cubes;

	std::queue<std::string> scrambleCache;
	std::queue<RubikAnimation*> movesQueue;

	void addMove(RubikAnimation* rubikAnim);

public:
	RubiksCube();
	~RubiksCube();

	void addMove(std::string move, float duration = 1.0f);
	void scramble(int maxMoves = 35);

	void loadTextures();

	void update(float deltaTime);
	void draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const;
	void solve();
};

#endif
