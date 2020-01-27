//#include "headers/preprop.h"
//#define GLFW_INCLUDE_VULKAN
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
//
//#include "VulkanAPIContext.h"
//
//std::vector<const char*> extensions;
//std::vector<const char*> layers;
//
//int main(int argc, char* argv[])
//{
//	initLogging(argc, argv);
//	LOG_SCOPE_FUNCTION(INFO);
//	VulkanAPIContext context(nullptr, true);
//
//	layers.push_back("VK_LAYER_KHRONOS_validation");
//	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//	extensions.push_back("VK_KHR_win32_surface");
//	extensions.push_back("VK_KHR_surface");
//	context.AttachExtensionList(extensions);
//	context.AttachLayerList(layers);
//	context.InitInstance();
//
//	GLFWwindow* window;
//
//	if(!glfwInit())
//		LOG_F(ERROR, "Failed to Init GLFW");
//
//
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//
//	window = glfwCreateWindow(800, 600, "Vulkan API Sandbox", NULL, NULL);
//
//	context.AttachWindowContext(glfwGetWin32Window(window));
//	context.InitSurface();
//	context.InitDevice();
//	if (!window)
//	{
//		glfwTerminate();
//		LOG_F(ERROR, "Failed to Create Window!");
//	}
//
//	glfwMakeContextCurrent(window);
//
//
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//	}
//}