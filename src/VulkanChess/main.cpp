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
#include <vulkan/vk_enum_string_helper.h>

#include <cstdlib>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `VulkanChess`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

constexpr uint32_t height = 600; // NOLINT(misc-include-cleaner)
constexpr uint32_t width = 800; // NOLINT(misc-include-cleaner)

class VulkanChessApp final {
public:
	VulkanChessApp() = default;

	void run() {
		initVulkan();
		initWindow();
		mainLoop();
		cleanup();
	}

private:
	void initVulkan() {
		VkResult const initResult = volkInitialize(); // NOLINT(misc-include-cleaner)
		if (VK_SUCCESS != initResult) { // NOLINT(misc-include-cleaner)
			fmt::print("Couldn't initialize Volk. Result code: {}\\n", string_VkResult(initResult)); // NOLINT(misc-include-cleaner)
			throw std::runtime_error("Couldn't initialize Volk.");
		}

		VkApplicationInfo appInfo{}; // NOLINT(misc-include-cleaner)
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // NOLINT(misc-include-cleaner)
		appInfo.pApplicationName = "Vulkan Chess";
		appInfo.applicationVersion = VK_MAKE_VERSION(VulkanChess::cmake::project_version_major, VulkanChess::cmake::project_version_minor, VulkanChess::cmake::project_version_patch); // NOLINT(misc-include-cleaner)
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // NOLINT(misc-include-cleaner)
		appInfo.apiVersion = VK_API_VERSION_1_0; // NOLINT(misc-include-cleaner)

		VkInstanceCreateInfo createInfo{}; // NOLINT(misc-include-cleaner)
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // NOLINT(misc-include-cleaner)
		createInfo.pApplicationInfo = &appInfo;
		setActiveExtensions(createInfo);
		createInfo.enabledLayerCount = 0;

		VkResult const createResult = vkCreateInstance(&createInfo, nullptr, &instance); // NOLINT(misc-include-cleaner)
		if (VK_SUCCESS != createResult) { // NOLINT(misc-include-cleaner)
			fmt::print("Couldn't initialize Volk. Result code: {}\\n", string_VkResult(initResult)); // NOLINT(misc-include-cleaner)
			throw std::runtime_error("Couldn't initialize Volk.");
		}
	}

	static void setActiveExtensions(VkInstanceCreateInfo& createInfo) { // NOLINT(misc-include-cleaner)
		uint32_t glfwExtensionCount = 0; // NOLINT(misc-include-cleaner)
		char const* const* const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
	}

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(width, height, "Vulkan Window", nullptr, nullptr);
	}

	void mainLoop() {
		while (0 == glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		glfwDestroyWindow(window);

		glfwTerminate();
	}

	GLFWwindow* window = nullptr;
	VkInstance instance = nullptr; // NOLINT(misc-include-cleaner)
};

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

		VulkanChessApp vcApp{};
		vcApp.run();

		return EXIT_SUCCESS;
	} catch (const std::exception &e) {
		fmt::print("Unhandled exception in main: {}\\n", e.what()); // NOLINT(misc-include-cleaner)
		return EXIT_FAILURE;
	}
}
