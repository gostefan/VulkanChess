#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define VK_NO_PROTOTYPES 

#include <CLI/CLI.hpp> // NOLINT(misc-include-cleaner)
#include <fmt/format.h> // NOLINT(misc-include-cleaner)
#include <GLFW/glfw3.h> // NOLINT(misc-include-cleaner)
#include <glm/vec4.hpp> // NOLINT(misc-include-cleaner)
#include <glm/mat4x4.hpp> // NOLINT(misc-include-cleaner)
#include <volk.h> // NOLINT(misc-include-cleaner)

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
		CLI::App app{ fmt::format("{} version {}", VulkanChess::cmake::project_name, VulkanChess::cmake::project_version) }; // NOLINT(misc-include-cleaner)

		std::optional<std::string> message;
		app.add_option("-m,--message", message, "A message to print back out");
		bool show_version = false; // NOLINT(misc-const-correctness)
		app.add_flag("--version", show_version, "Show version information");

		CLI11_PARSE(app, argc, argv); // NOLINT(misc-include-cleaner)

		if (show_version) {
			fmt::print("{}\n", VulkanChess::cmake::project_version); // NOLINT(misc-include-cleaner)
			return EXIT_SUCCESS;
		}

		VkResult initResult = volkInitialize(); // NOLINT(misc-include-cleaner)
		if (VK_SUCCESS != initResult) { // NOLINT(misc-include-cleaner)
			fmt::print("Couldn't initialize Volk. Result code: {}\\n", initResult); // NOLINT(misc-include-cleaner)
			return initResult;
		}

		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

		uint32_t extensionCount = 0; // NOLINT(misc-include-cleaner)
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		fmt::print("{} extensions supported\n", extensionCount); // NOLINT(misc-include-cleaner)

		glm::mat4 matrix; // NOLINT(misc-include-cleaner)
		glm::vec4 vec; // NOLINT(misc-include-cleaner)
		[[maybe_unused]] auto test = matrix * vec;

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}

		glfwDestroyWindow(window );

		glfwTerminate();

		return EXIT_SUCCESS;
	} catch (const std::exception &e) {
		fmt::print("Unhandled exception in main: {}\\n", e.what()); // NOLINT(misc-include-cleaner)
	}
}
