#include "2dshapes_definitions.h"

static const char* fragmentShaderUniformSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 someColor;\n"
"void main()\n"
"{\n"
"   FragColor = someColor;\n"
"}\n\0";

int shader_basics() {

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -2;
	}

	// Make the OpenGL context for the current window
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers with GLAD after context creation
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -3;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Define the input vertex data for a triangle
	float vertices[] = {
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
	};

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Upload vertex data to the currently bound GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//copy data to gpu
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// instructions for interpreting the first vertex attribute
	glEnableVertexAttribArray(0);// enable the vertex attribute for the vao

	// Create and compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderUniformSource, NULL);
	glCompileShader(fragmentShader);

	// Check for shader compilation errors
	int vertexShaderSuccess;
	int fragmentShaderSuccess;
	char vertexShaderInfoLog[512];
	char fragmentShaderInfoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);

	if (!vertexShaderSuccess || !fragmentShaderSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShaderInfoLog << std::endl;
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderInfoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int shaderProgramSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramSuccess);
	if (!shaderProgramSuccess)
	{
		char shaderProgramInfoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shaderProgramInfoLog << std::endl;
	}

	int vertexColorLocation = glGetUniformLocation(shaderProgram, "someColor");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		float timevalue = glfwGetTime();
		float greenValue = sin(timevalue) / 2.0f + 0.5f;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // no native support for overloading in opengl
		glDrawArrays(GL_TRIANGLES,0,3);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
