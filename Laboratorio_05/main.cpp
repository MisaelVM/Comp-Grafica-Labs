#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void normalize_positions(std::vector<float>& positions);

const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n\0";

const char* fragmentShaderSource =
	"#version 330 core\n"
	"uniform vec4 u_Color;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"	color = u_Color;\n"
	"}\n";

const char* circleFragmentShaderSource =
	"#version 330 core\n"
	"uniform vec2 u_Resolution;\n"
	"uniform vec3 u_CircleColor;\n"
	"uniform vec2 u_CircleDim;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = gl_FragCoord.xy / u_Resolution.xy * 2.0 - 1.0;\n"
	"	float aspect = u_Resolution.x / u_Resolution.y;\n"
	"	uv *= 7;\n"
	"	uv.x -= 1.8;\n"
	"	uv.y *= 0.9;\n"
	"	uv.y -= 2;\n"
	"	color.rg = uv;\n"
	"	color.b = 0.0;\n"
	"	float thickness = 0.5;\n"
	"	float distance = 1.0 - length(uv);\n"
	"	float fade = 0.005;\n"
	"	vec3 col = vec3(smoothstep(0.0, fade, distance));\n"
	"	col *= vec3(smoothstep(thickness, thickness - fade, distance));\n"
	"	color.rgb = col;\n"
	"	color.rgb *= u_CircleColor;\n"
	"}\n\0";

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laboratorio 5", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	std::vector<float> vertices;

	std::ifstream fileReader("vertex_data.txt");
	if (!fileReader.is_open()) {
		std::cout << "Failed to read vertex data\n";
		return -1;
	}
	float position;
	while (fileReader >> position)
		vertices.push_back(position);
	fileReader.close();

	normalize_positions(vertices);

	std::vector<unsigned int> indices;

	fileReader.open("index_data.txt");
	if (!fileReader.is_open()) {
		std::cout << "Failed to read index data\n";
		return -1;
	}
	unsigned int index;
	while (fileReader >> index)
		indices.push_back(index);
	fileReader.close();


	// Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Linking Shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}

	// Circle Shader
	unsigned int circleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(circleFragmentShader, 1, &circleFragmentShaderSource, nullptr);
	glCompileShader(circleFragmentShader);
	glGetShaderiv(circleFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(circleFragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	unsigned int circleShaderProgram = glCreateProgram();
	glAttachShader(circleShaderProgram, vertexShader);
	glAttachShader(circleShaderProgram, circleFragmentShader);
	glLinkProgram(circleShaderProgram);
	glGetProgramiv(circleShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(circleShaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(circleFragmentShader);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	// Uniforms
	int colorLocation = glGetUniformLocation(shaderProgram, "u_Color");
	int resolutionLocation = glGetUniformLocation(circleShaderProgram, "u_Resolution");
	int circleColorLocation = glGetUniformLocation(circleShaderProgram, "u_CircleColor");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glUseProgram(circleShaderProgram);
		glUniform2f(resolutionLocation, SCR_WIDTH, SCR_HEIGHT);
		glUniform3f(circleColorLocation, 0.91, 0.93, 0.16);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(840 * sizeof(unsigned int)));

		glUseProgram(shaderProgram);
		glUniform4f(colorLocation, 0.93f, 0.3f, 0.92f, 1.0f);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

		glUniform4f(colorLocation, 0.41f, 0.06f, 0.84f, 1.0f);
		glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, (const void*)(48 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
		glDrawElements(GL_TRIANGLES, 156, GL_UNSIGNED_INT, (const void*)(195 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.33f, 0.88f, 0.97f, 1.0f);
		glDrawElements(GL_TRIANGLES, 114, GL_UNSIGNED_INT, (const void*)(351 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.23f, 0.72f, 0.39f, 1.0f);
		glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, (const void*)(465 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.52f, 0.94f, 0.63f, 1.0f);
		glDrawElements(GL_TRIANGLES, 66, GL_UNSIGNED_INT, (const void*)(519 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.43f, 0.37f, 0.49f, 1.0f);
		glDrawElements(GL_TRIANGLES, 126, GL_UNSIGNED_INT, (const void*)(585 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.55f, 0.55f, 0.56f, 1.0f);
		glDrawElements(GL_TRIANGLES, 129, GL_UNSIGNED_INT, (const void*)(711 * sizeof(unsigned int)));

		glUniform4f(colorLocation, 0.58f, 0.9f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, (const void*)(846 * sizeof(unsigned int)));
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clearing resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(circleShaderProgram);
	glDeleteProgram(shaderProgram);

	// Terminate
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width, SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void normalize_positions(std::vector<float>& positions) {
	for (int i = 0; i < (int)positions.size(); i += 2) {
		positions[i] = (positions[i] / 18.f * 2) - 1;
		positions[i + 1] = (positions[i + 1] / 14.08f * 2) - 1;
	}
}
