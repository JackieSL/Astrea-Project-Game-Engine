#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <loguru.hpp>
class AstreaWindow
{
private:
	GLFWwindow* window;
	//GLFWwindow** children;
public:
	AstreaWindow()
	{
		window = nullptr;
	}
	AstreaWindow(std::string name, uint32_t sizeX, uint32_t sizeY)
	{
		LOG_IF_F(ERROR, glfwInit() == false, "Failed to init GLFW...");
		window = glfwCreateWindow(sizeX, sizeY, name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
	}

	virtual void Run()
	{
		while (!glfwWindowShouldClose(window))
		{
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
};