//3. Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; 
//draw both triangles again where one outputs the color yellow. This draws 2 triangles so they don't overlap.

#include "2dshapes_definitions.h"

int ex3()
{
	// Initialize GLFW
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

	float vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.8f, 0.2f, 0.0f,
		-0.1f, 0.2f, 0.0f,
		// 2nd triangle
		0.5f, -0.5f, 0.0f,
		0.8f,  -0.2f, 0.0f,
		0.1f,  -0.2f, 0.0f
	};

	unsigned int VBO, VAO1, VAO2;

	glGenVertexArrays(1, &VAO1);
	glGenVertexArrays(1, &VAO2);

	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);// unbind VAO1

	// create 2nd VAO for the 2nd triangle
	glBindVertexArray(VAO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * 3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);// unbind VAO2

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int fragmentShaderYellow;
	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	int vertexShaderSuccess;
	int fragmentShaderSuccess;
	int fragmentShaderYellowSuccess;
	char vertexShaderInfoLog[512];
	char fragmentShaderInfoLog[512];
	char fragmentShaderYellowInfoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);
	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &fragmentShaderYellowSuccess);

	if (!vertexShaderSuccess || !fragmentShaderSuccess || !fragmentShaderYellowSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, fragmentShaderYellowInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexShaderInfoLog << std::endl;
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentShaderInfoLog << std::endl;
		std::cout << "ERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n" << fragmentShaderYellowInfoLog << std::endl;
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

	unsigned int shaderProgramYellow;
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderYellow);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}