#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define VK_NO_PROTOTYPES 

#include <CLI/CLI.hpp>
#include <fmt/format.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <volk.h>

#include <cstdlib>
#include <exception>
#include <optional>
#include <string>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `VulkanChess`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

int main(int argc, const char **argv) {
	try {
		CLI::App app{ fmt::format("{} version {}", VulkanChess::cmake::project_name, VulkanChess::cmake::project_version) };

		std::optional<std::string> message;
		app.add_option("-m,--message", message, "A message to print back out");
		bool show_version = false;
		app.add_flag("--version", show_version, "Show version information");

		CLI11_PARSE(app, argc, argv);

		if (show_version) {
			fmt::print("{}\n", VulkanChess::cmake::project_version);
			return EXIT_SUCCESS;
		}

		VkResult initResult = volkInitialize();
		if (VK_SUCCESS != initResult) {
			fmt::print("Couldn't initialize Volk. Result code: {}\\n", initResult);
			return initResult;
		}

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		fmt::print("{} extensions supported\n", extensionCount);

		glm::mat4 matrix;
		glm::vec4 vec;
		[[maybe_unused]] auto test = matrix * vec;

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}

		glfwDestroyWindow(window );

		glfwTerminate();

		return EXIT_SUCCESS;
	} catch (const std::exception &e) {
		fmt::print("Unhandled exception in main: {}\\n", e.what());
	}
}
