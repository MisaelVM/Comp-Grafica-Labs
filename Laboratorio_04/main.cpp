#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#define pi 3.14159265359

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec4 position;\n"
	"layout (location = 1) in vec4 color;\n"
	"out vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"	ourColor = color;\n"
	"}\n\0";

const char* fragmentShaderSource =
	"#version 330 core\n"
	"#define pi 3.14159265359\n"
	"uniform float u_Phase;\n"
	"uniform float u_Amp;\n"
	"uniform vec4 u_Color1;\n"
	"uniform vec4 u_Color2;\n"
	"out vec4 FragColor;\n"
	"float f(vec2 uv);\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = gl_FragCoord.xy / vec2(800, 600);\n"
	"	float f1 = f(uv);\n"
	"	float f2 = f(uv);\n"
	"	vec4 color = vec4(f1) * u_Color1 + vec4(f2) * u_Color2;\n"
	"	FragColor = color;\n"
	"}\n"
	"float f(vec2 uv) {\n"
	"	vec2 p = vec2(0.5) - uv;\n"
	"	float radius = length(p);\n"
	"	float angle = atan(p.x, p.y);\n"
	"	float f = sin(radius * pi * u_Amp + sin(angle * u_Amp + u_Phase) + sin(uv.x * pi * 3.3f * u_Amp + u_Phase) + u_Phase);\n"
	"	f += sin(uv.x * 1.3f * u_Amp + u_Phase) + sin(angle * u_Amp + u_Phase);\n"
	"	return abs(f);\n"
	"}\n\0";

const char* fragmentShader1Source =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = ourColor;\n"
	"}\n\0";

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Laboratorio 4", nullptr, nullptr);
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

	// Vertex data
	float vertices[] = {
		// Positions	// Colors
		-0.75f, -0.75f,	1.0f, 0.0f, 0.0f,
		 0.75f, -0.75f,	0.0f, 1.0f, 0.0f,
		 0.00f,  0.75f,	0.0f, 0.0f, 1.0f
	};

	// Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Uniforms
	int PhaseLocation = glGetUniformLocation(shaderProgram, "u_Phase");
	int AmplitudeLocation = glGetUniformLocation(shaderProgram, "u_Amp");
	int Color1Location = glGetUniformLocation(shaderProgram, "u_Color1");
	int Color2Location = glGetUniformLocation(shaderProgram, "u_Color2");
	
	glUseProgram(shaderProgram);
	glUniform1f(PhaseLocation, 0.0f);
	glUniform1f(AmplitudeLocation, 0.0f);
	glUniform4f(Color1Location, 0.2f, 0.7f, 0.9f, 1.0f);
	glUniform4f(Color2Location, 0.7f, 0.9f, 1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float phase = 0.0f;
	float num = 15.0f;

	float phaseIncrement = 0.05f;
	float numIncrement = 0.05f;

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.33f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		if (num >= 20.0f || num <= 10.0f)
			numIncrement *= -1;

		phase += phaseIncrement;
		num += numIncrement;

		if (phase > 2 * pi)
			phase -= 2 * pi;

		glUniform1f(PhaseLocation, phase);
		glUniform1f(AmplitudeLocation, num);
		
		// Drawing shape
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clearing resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
