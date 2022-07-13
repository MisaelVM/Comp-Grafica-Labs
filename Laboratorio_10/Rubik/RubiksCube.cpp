#include "RubiksCube.h"

RubiksCube::RubiksCube() {
	std::vector<glm::vec3> cubPositions;
	int i, j, k;
	for (i = -1; i < 2; ++i) {
		for (j = -1; j < 2; ++j) {
			for (k = -1; k < 2; ++k) {
				if (!k && !j && !i) continue;
				cubPositions.push_back(glm::vec3(k, j, i));
			}
		}
	}

	cubes.resize(26);

	for (int i = 0; i < 26; ++i)
		cubes[i].position = cubPositions[i];
	
	for (int i = 0; i < 6; ++i)
		std::fill_n(textures[i], 9, nullptr);

	rb = rb::RubikCube(3);
}

RubiksCube::~RubiksCube() {
	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 9; ++j)
			delete textures[i][j];
}

void RubiksCube::addMove(std::string move, float duration) {
	scrambleCache.push(move);
	movesQueue.push(new RubikAnimation(cubes, move, duration));
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
		scrambleCache.push(allowedMoves[randomMove]);
		movesQueue.push(new RubikAnimation(cubes, allowedMoves[randomMove], (allowedMoves[randomMove][1] == '2' ? 0.3f : 0.15f)));
	}
}

void RubiksCube::loadTextures() {
	for (int i = 0; i < 6; ++i) {
		std::string path = "Rubik/Textures/";
		switch (i) {
		case 0: path += "Yellow/"; break;
		case 1: path += "Orange/"; break;
		case 2: path += "White/"; break;
		case 3: path += "Red/"; break;
		case 4: path += "Green/"; break;
		case 5: path += "Blue/"; break;
		}

		textures[i][0] = new Texture(path + "UL.jpg");
		textures[i][1] = new Texture(path + "UM.jpg");
		textures[i][2] = new Texture(path + "UR.jpg");
		textures[i][3] = new Texture(path + "ML.jpg");
		textures[i][4] = new Texture(path + "MM.jpg");
		textures[i][5] = new Texture(path + "MR.jpg");
		textures[i][6] = new Texture(path + "DL.jpg");
		textures[i][7] = new Texture(path + "DM.jpg");
		textures[i][8] = new Texture(path + "DR.jpg");
	}

	int face = 0, x = 0, y = 0;
	for (int i = 0; i < 26; ++i) {
		if (abs(cubes[i].position.z + 1.0f) < 1e-3) {
			face = 0;
			if (abs(cubes[i].position.y - 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.y + 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;
			
			if (abs(cubes[i].position.x - 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.x + 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
		else if (abs(cubes[i].position.z - 1.0f) < 1e-3) {
			face = 2;
			if (abs(cubes[i].position.y - 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.y + 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;

			if (abs(cubes[i].position.x + 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.x - 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
		if (abs(cubes[i].position.y - 1.0f) < 1e-3) {
			face = 4;
			if (abs(cubes[i].position.z + 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.z - 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;

			if (abs(cubes[i].position.x + 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.x - 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
		else if (abs(cubes[i].position.y + 1.0f) < 1e-3) {
			face = 5;

			if (abs(cubes[i].position.z - 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.z + 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;

			if (abs(cubes[i].position.x + 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.x - 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
		if (abs(cubes[i].position.x - 1.0f) < 1e-3) {
			face = 1;
			if (abs(cubes[i].position.y - 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.y + 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;

			if (abs(cubes[i].position.z - 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.z + 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
		else if (abs(cubes[i].position.x + 1.0f) < 1e-3) {
			face = 3;
			if (abs(cubes[i].position.y - 1.0f) < 1e-3)
				y = 0;
			else if (abs(cubes[i].position.y + 1.0f) < 1e-3)
				y = 2;
			else
				y = 1;

			if (abs(cubes[i].position.z + 1.0f) < 1e-3)
				x = 0;
			else if (abs(cubes[i].position.z - 1.0f) < 1e-3)
				x = 2;
			else
				x = 1;

			cubes[i].faceTextures[face] = textures[face][3 * y + x];
		}
	}
}

void RubiksCube::addMove(RubikAnimation* rubikAnim) {
	movesQueue.push(rubikAnim);
}

void RubiksCube::update(float deltaTime) {
	// Check pending moves in the queue
	if (!movesQueue.empty()) {
		// Move is available
		if (!movesQueue.front()->completed) {
			// Move has not been started
			if (!movesQueue.front()->started)
				movesQueue.front()->start();
			else // The move animation is still playing
				movesQueue.front()->update(deltaTime);
		}
		else {
			// Animation has been completed
			delete movesQueue.front();
			movesQueue.pop();
		}
	}
}

void RubiksCube::draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const {
	VAO.bind();
	EBO.bind();
	shaderProgram.bind();

	for (int i = 0; i < 26; ++i)
		cubes[i].draw(VAO, EBO, shaderProgram);
}

void RubiksCube::solve() {
	if (scrambleCache.empty())
		return;

	// Scramble data structure
	std::string moves = scrambleCache.front(); scrambleCache.pop();
	while (!scrambleCache.empty()) {
		moves += " ";
		moves += scrambleCache.front(); scrambleCache.pop();
	}

	rb.Move(moves);

	// Solve Rubik's Cube
	rb::RubikCubeSolver* solver = new rb::RubikCube3BasicSolver(rb);
	moves = solver->Solve();

	// Solve structure
	rb.Move(moves);

	// Load animations
	std::cout << "Solution:\n" << moves << "\n";
	size_t pos = 0;
	std::string move;
	while ((pos = moves.find(" ")) != std::string::npos) {
		move = moves.substr(0, pos);
		movesQueue.push(new RubikAnimation(cubes, move, (move[1] == '2' ? 0.3f : 0.15f)));
		moves.erase(0, pos + 1);
	}
	if (moves[0]) {
		move = moves.substr(0);
		movesQueue.push(new RubikAnimation(cubes, move, (move[1] == '2' ? 0.3f : 0.15f)));
	}

	delete solver;
}

RubikAnimation::RubikAnimation(std::vector<Cube>& cubs, std::string move, float duration)
	: cubes(cubs),
	  move(move), rotationAxis(glm::vec3()), rotationAngle(0.0f),
	  duration(std::max(duration, 0.001f)), timeSoFar(0.0f) {}

void RubikAnimation::start() {
	// Set the kind of rotation here...
	started = true;
	initialCubes = cubes;

	if (move[0] == 'R' || move[0] == 'r' || move[0] == 'X')
		rotationAxis = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
	else if (move[0] == 'U' || move[0] == 'u' || move[0] == 'Y')
		rotationAxis = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	else if (move[0] == 'F' || move[0] == 'S' || move[0] == 'f' || move[0] == 'Z')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	else if (move[0] == 'L' || move[0] == 'M' || move[0] == 'l')
		rotationAxis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
	else if (move[0] == 'D' || move[0] == 'E' || move[0] == 'd')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	else if (move[0] == 'B' || move[0] == 'b')
		rotationAxis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

	if (move[1] == '\'')
		rotationAngle = glm::radians(-90.0f);
	else if (move[1] == '2')
		rotationAngle = glm::radians(-180.0f);
	else
		rotationAngle = glm::radians(90.0f);

	if (move[0] == 'X' || move[0] == 'Y' || move[0] == 'Z') {
		interestingIndices.resize(26);
		std::iota(interestingIndices.begin(), interestingIndices.end(), 0);
		return;
	}

	for (int i = 0; i < 26; ++i) {
		if (move[0] == 'R' && abs(cubes[i].position.x - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'U' && abs(cubes[i].position.y - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'F' && abs(cubes[i].position.z - 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'L' && abs(cubes[i].position.x + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'D' && abs(cubes[i].position.y + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'B' && abs(cubes[i].position.z + 1.0f) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'M' && abs(cubes[i].position.x) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'E' && abs(cubes[i].position.y) < 1e-3)
			interestingIndices.push_back(i);
		else if (move[0] == 'S' && abs(cubes[i].position.z) < 1e-3)
			interestingIndices.push_back(i);

		else if (move[0] == 'r' && !(abs(cubes[i].position.x + 1.0f) < 1e-3))
			interestingIndices.push_back(i);
		else if (move[0] == 'u' && !(abs(cubes[i].position.y + 1.0f) < 1e-3))
			interestingIndices.push_back(i);
		else if (move[0] == 'f' && !(abs(cubes[i].position.z + 1.0f) < 1e-3))
			interestingIndices.push_back(i);
		else if (move[0] == 'l' && !(abs(cubes[i].position.x - 1.0f) < 1e-3))
			interestingIndices.push_back(i);
		else if (move[0] == 'd' && !(abs(cubes[i].position.y - 1.0f) < 1e-3))
			interestingIndices.push_back(i);
		else if (move[0] == 'b' && !(abs(cubes[i].position.z - 1.0f) < 1e-3))
			interestingIndices.push_back(i);
	}
}

void RubikAnimation::update(float deltaTime) {
	// Interpolation
	timeSoFar += deltaTime;
	float t = timeSoFar / duration;
	if (t > 1.0f) t = 1.0f;

	glm::quat quaternion = glm::quat(glm::cos(rotationAngle / 2), glm::sin(rotationAngle / 2) * rotationAxis);

	// Update model
	for (auto& i : interestingIndices) {
		glm::quat quatFrom = initialCubes[i].rotation;
		glm::quat quatTo = quaternion * quatFrom;

		glm::quat identity = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		// Interpolating rotation
		cubes[i].rotation = glm::slerp(quatFrom, quatTo, t);
		// Interpolating position
		cubes[i].position = glm::slerp(identity, quaternion, t) * glm::vec4(initialCubes[i].position, 1.0f);
	}

	if (timeSoFar >= duration) {
		// Set new model transforms and positions
		for (auto& i : interestingIndices) {
			cubes[i].rotation = quaternion * initialCubes[i].rotation;
			cubes[i].position = quaternion * glm::vec4(initialCubes[i].position, 1.0f);
		}

		completed = true;
	}
}

Cube::Cube() {
	for (int i = 0; i < 6; ++i)
		faceTextures[i] = nullptr;
}

void Cube::draw(const VertexArray& VAO, const ElementBuffer& EBO, Shader& shaderProgram) const {
	VAO.bind();
	EBO.bind();
	shaderProgram.bind();

	shaderProgram.setUniformMatrix4f("u_Trans", glm::translate(glm::mat4(1.0f), position));
	shaderProgram.setUniformMatrix4f("u_Rot", glm::mat4_cast(rotation));
	shaderProgram.setUniformMatrix4f("u_Scale", glm::scale(glm::mat4(1.0f), scale));

	shaderProgram.setUniform1i("u_Texture", 0);

	for (int face = 0; face < 6; ++face) {
		if (faceTextures[face])
			faceTextures[face]->bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(6 * face * sizeof(unsigned int)));

		if (faceTextures[face])
			faceTextures[face]->unbind();
	}
}
