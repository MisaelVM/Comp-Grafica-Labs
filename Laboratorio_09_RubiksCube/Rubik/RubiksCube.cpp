#include "RubiksCube.h"

RubiksCube::RubiksCube() {
	cubPositions = {
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3( 0.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  0.0f, -1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 1.0f,  0.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3( 0.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
		glm::vec3( 1.0f, -1.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 1.0f,  1.0f,  0.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 0.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 1.0f,  0.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3( 0.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
	};
	
	cubTransforms.resize(26);
	for (int i = 0; i < 26; ++i)
		cubTransforms[i] = glm::translate(glm::mat4(1.0f), cubPositions[i]);
}

void RubiksCube::addMovement(std::string movement, float duration) {
	scrambleCache.push_back(movement);
	movementsQueue.push(new RubikAnimation(cubPositions, cubTransforms, movement, duration));
}

void RubiksCube::scramble(int maxMovements) {
	std::vector<std::string> allowedMoves = {
		"R", "R'", "R2",
		"L", "L'", "L2",
		"U", "U'", "U2",
		"D", "D'", "D2",
		"F", "F'", "F2",
		"B", "B'", "B2",
	};

	for (int i = 0; i < maxMovements; ++i) {
		int randomMove = rand() % allowedMoves.size();
		scrambleCache.push_back(allowedMoves[randomMove]);
		movementsQueue.push(new RubikAnimation(cubPositions, cubTransforms, allowedMoves[randomMove], (allowedMoves[randomMove][1] == '2' ? 0.3f : 0.15f)));
	}
}

void RubiksCube::addMovement(RubikAnimation* rubikAnim) {
	movementsQueue.push(rubikAnim);
}

void RubiksCube::update(float deltaTime) {
	// Check pending movements in the queue
	if (!movementsQueue.empty()) {
		// Movement is available
		if (!movementsQueue.front()->completed) {
			// Movement has not been started
			if (!movementsQueue.front()->started) {
				movementsQueue.front()->start();
				movementsQueue.front()->started = true;
			}
			else // The movement animation is still playing
				movementsQueue.front()->update(deltaTime);
		}
		else {
			// Animation has been completed
			delete movementsQueue.front();
			movementsQueue.pop();
		}
	}
}

void RubiksCube::draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const {
	VAO.bind();
	EBO.bind();
	shaderProgram.bind();

	for (int i = 0; i < 26; ++i) {
		shaderProgram.setUniformMatrix4f("u_Model", cubTransforms[i]);

		// Render north face
		shaderProgram.setUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // Yellow
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Render east face
		shaderProgram.setUniform4f("u_Color", 1.0f, 0.5f, 0.2f, 1.0f); // Orange
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(6 * sizeof(unsigned int)));

		// Render south face
		shaderProgram.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f); // White
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(12 * sizeof(unsigned int)));

		// Render west face
		shaderProgram.setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f); // Red
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(18 * sizeof(unsigned int)));

		// Render top face
		shaderProgram.setUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f); // Green
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(24 * sizeof(unsigned int)));

		// Render bottom face
		shaderProgram.setUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f); // Blue
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(30 * sizeof(unsigned int)));
	}
}

void RubiksCube::solve() {
	for (auto itr = scrambleCache.rbegin(); itr != scrambleCache.rend(); ++itr) {
		std::string move = *itr;
		float animTime = 0.15f;
		if (move[1] == '\0')
			move.resize(2, '\'');
		else if (move[1] == '\'')
			move.resize(1);
		else
			animTime = 0.3f;

		movementsQueue.push(new RubikAnimation(cubPositions, cubTransforms, move, animTime));
	}

	scrambleCache.clear();
}

RubikAnimation::RubikAnimation(std::vector<glm::vec3>& pos, std::vector<glm::mat4>& trans, std::string movement, float duration)
	: positions(pos), transforms(trans),
	movement(movement), rotationAxis(glm::vec3()), rotationAngle(0.0f),
	  duration(std::max(duration, 0.001f)), timeSoFar(0.0f) {}

void RubikAnimation::start() {
	// Set the kind of rotation here...
	started = true;
	initialTransforms = transforms;

	if (movement[0] == 'R')
		rotationAxis = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
	else if (movement[0] == 'U')
		rotationAxis = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	else if (movement[0] == 'F')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	else if (movement[0] == 'L')
		rotationAxis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
	else if (movement[0] == 'D')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	else if (movement[0] == 'B')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

	if (movement[1] == '\'')
		rotationAngle = glm::radians(-90.0f);
	else if (movement[1] == '2')
		rotationAngle = glm::radians(180.0f);
	else
		rotationAngle = glm::radians(90.0f);

	for (int i = 0; i < 26; ++i) {
		if (movement[0] == 'R' && abs(positions[i].x - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (movement[0] == 'U' && abs(positions[i].y - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (movement[0] == 'F' && abs(positions[i].z - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (movement[0] == 'L' && abs(positions[i].x + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (movement[0] == 'D' && abs(positions[i].y + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (movement[0] == 'B' && abs(positions[i].z + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
	}
}

void RubikAnimation::update(float deltaTime) {
	// Interpolation
	timeSoFar += deltaTime;
	float t = timeSoFar / duration;
	if (t > 1.0f) t = 1.0f;

	float angleRad = deltaTime * rotationAngle / duration;
	glm::quat quaternion = glm::quat(glm::cos(angleRad / 2), glm::sin(angleRad / 2) * rotationAxis);
	glm::mat4 rotation = glm::mat4_cast(quaternion);

	for (auto& i : interestingIndices) {
		glm::mat4 model = rotation * transforms[i];
		transforms[i] = model;
	}

	// Update model
	// ...

	if (timeSoFar >= duration) {
		// Set new model transforms and positions
		// ...	

		quaternion = glm::quat(glm::cos(rotationAngle / 2), glm::sin(rotationAngle / 2) * rotationAxis);
		glm::mat4 rotation = glm::mat4_cast(quaternion);

		for (auto& i : interestingIndices) {
			glm::vec4 newPos = rotation * glm::vec4(positions[i], 1.0f);
			positions[i] = newPos;
			glm::mat4 model = rotation * initialTransforms[i];
			transforms[i] = model;
		}

		completed = true;
	}

}
