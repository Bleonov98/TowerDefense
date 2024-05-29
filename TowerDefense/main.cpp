#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"

// settings
const unsigned int SCR_WIDTH = 1600, SCR_HEIGHT = 900;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);

Game TD(SCR_WIDTH, SCR_HEIGHT);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tower Defense", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	TD.Init();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		TD.ProcessInput(deltaTime);
		TD.Update(deltaTime);

		TD.Render(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS || TD.close)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) 
			TD.Keys[key] = true;
		else if (action == GLFW_RELEASE) {
			TD.Keys[key] = false;
			TD.KeysProcessed[key] = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < 8)
	{
		if (action == GLFW_PRESS) TD.mouseKeys[button] = true;
		else if (action == GLFW_RELEASE) { 
			TD.mouseKeys[button] = false;
			TD.mKeysProcessed[button] = false;
		}
		
		double xpos, ypos;

		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		
		TD.xMouse = xpos;
		TD.yMouse = ypos;
	}
}