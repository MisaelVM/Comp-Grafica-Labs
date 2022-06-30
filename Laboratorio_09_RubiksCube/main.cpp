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

#include "Rubik/RubiksCube.h"

RubiksCube Rubik;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window, float deltaTime);

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

	GLFWwindow* window = glfwCreateWindow(800, 600, "Rubik's Cube", nullptr, nullptr);
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

	float positions[] = {
		-0.5f, -0.5f, -0.5f, // LDB
		-0.5f,  0.5f, -0.5f, // LUB
		 0.5f,  0.5f, -0.5f, // RUB
		 0.5f, -0.5f, -0.5f, // RDB
		-0.5f, -0.5f,  0.5f, // LDF
		-0.5f,  0.5f,  0.5f, // LUF
		 0.5f,  0.5f,  0.5f, // RUF
		 0.5f, -0.5f,  0.5f, // RDF
	};

	unsigned int indices[] = {
		// North
		3, 2, 1,
		1, 0, 3,

		// East
		7, 6, 2,
		2, 3, 7,

		// South
		4, 5, 6,
		6, 7, 4,

		// West
		0, 1, 5,
		5, 4, 0,

		// Top
		5, 1, 2,
		2, 6, 5,

		// Bottom
		0, 4, 7,
		7, 3, 0
	};

	srand(time(0));

	VertexArray VAO;
	VertexBuffer VBO(positions, 24 * sizeof(float));
	ElementBuffer EBO(indices, 36 * sizeof(float));

	VAO.bind();
	VAO.addBuffer(VBO, 0);

	Shader shaderProgram("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

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
		cameraAngleY += 2.0f * deltaTime;
		if (cameraAngleY > glm::radians(89.0f))
			cameraAngleY = glm::radians(89.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraAngleY -= 2.0f * deltaTime;
		if (cameraAngleY < glm::radians(-89.0f))
			cameraAngleY = glm::radians(-89.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraAngleX += 2.0f * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraAngleX -= 2.0f * deltaTime;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("R'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("R2", 0.5f);
		else
			Rubik.addMovement("R", 0.25f);

	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("U'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("U2", 0.5f);
		else
			Rubik.addMovement("U", 0.25f);

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("F'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("F2", 0.5f);
		else
			Rubik.addMovement("F", 0.25f);

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("L'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("L2", 0.5f);
		else
			Rubik.addMovement("L", 0.25f);

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("D'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("D2", 0.5f);
		else
			Rubik.addMovement("D", 0.25f);

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			Rubik.addMovement("B'", 0.25f);
		else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
				 glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			Rubik.addMovement("B2", 0.5f);
		else
			Rubik.addMovement("B", 0.25f);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		Rubik.scramble();

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		Rubik.solve();
}
