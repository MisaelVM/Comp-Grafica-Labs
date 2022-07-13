#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "GL/Shader.h"
#include "GL/VertexArray.h"
#include "GL/VertexBuffer.h"
#include "GL/ElementBuffer.h"
#include "GL/Texture.h"

#include "Rubik/RubiksCube.h"

RubiksCube Rubik;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window, float deltaTime);

bool doubleRot = false;

// Camera
float radius = 10.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, radius);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(1200, 900, "Rubik's Cube", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	float vertices[] = {
		// positions			// texture coordinates
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f
	};

	unsigned int indices[] = {
		// North
		3, 2, 1,
		1, 0, 3,

		// East
		7, 6, 4,
		4, 5, 7,

		// South
		8, 9, 10,
		10, 11, 8,

		// West
		14, 15, 13,
		13, 12, 14,

		// Top
		16, 15, 4,
		4, 17, 16,

		// Bottom
		14, 18, 19,
		19, 5, 14
	};

	srand(static_cast<unsigned int>(time(0)));

	VertexArray VAO;
	VertexBuffer VBO(vertices, 20 * 5 * sizeof(float));
	ElementBuffer EBO(indices, 36 * sizeof(float));

	VAO.bind();
	VAO.addBuffer(VBO, 0);

	Shader shaderProgram("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	Rubik.loadTextures();

	shaderProgram.bind();
	shaderProgram.setUniformMatrix4f("u_Proj", proj);
	shaderProgram.setUniformMatrix4f("u_View", view);

	VAO.unbind();
	VBO.unbind();
	EBO.unbind();
	shaderProgram.unbind();

	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;
	float start = 0.0f, end = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		processInput(window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Rubik.update(deltaTime);
		Rubik.draw(VAO, EBO, shaderProgram);

		glm::mat4 cameraRotation = glm::rotate(glm::mat4(1.0f), cameraAngleX, glm::vec3(0.0f, 1.0f, 0.0f));
		cameraRotation = glm::rotate(cameraRotation, cameraAngleY, glm::vec3(-1.0f, 0.0f, 0.0f));
		cameraRotation = glm::translate(cameraRotation, glm::vec3(0.0f, 0.0f, radius));

		cameraPos = cameraRotation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		shaderProgram.bind();
		shaderProgram.setUniformMatrix4f("u_View", view);

		glfwSwapBuffers(window);
		glfwPollEvents();

		end = (float)glfwGetTime();
		deltaTime = end - start;
		start = end;
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		// Mover hacia arriba
		cameraAngleY += 2.0f * deltaTime;
		if (cameraAngleY > glm::radians(89.0f))
			cameraAngleY = glm::radians(89.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		// Mover hacia abajo
		cameraAngleY -= 2.0f * deltaTime;
		if (cameraAngleY < glm::radians(-89.0f))
			cameraAngleY = glm::radians(-89.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		// Mover hacia la derecha
		cameraAngleX += 2.0f * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		// Mover hacia la izquierda
		cameraAngleX -= 2.0f * deltaTime;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::string movement;
	float animDuration = 0.25f;

	// Face rotations
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		movement = "R";
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		movement = "U";
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		movement = "F";
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		movement = "L";
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		movement = "D";
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		movement = "B";

	// Double layer rotations
	if (doubleRot && movement[0])
		movement = tolower(movement[0]);

	// Slice turns
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		movement = "M";
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		movement = "E";
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		movement = "S";

	// Cube reorientation
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
		movement = "X";
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		movement = "Y";
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		movement = "Z";

	if (movement[0]) {
		// CounterClockWise Rotations
		if (mods == GLFW_MOD_SHIFT)
			movement += "'";
		// 180 degrees rotations
		else if (mods == GLFW_MOD_CONTROL) {
			movement += "2";
			animDuration = 0.5f;
		}

		Rubik.addMove(movement, animDuration);
	}

	// Enable/disable double layer rotations
	if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS)
		doubleRot ^= true;

	// Scramble cube
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		Rubik.scramble();

	// Solve cube
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		Rubik.solve();
}
