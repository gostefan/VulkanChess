#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#define VK_NO_PROTOTYPES

#include "Window.h"

#include <fmt/format.h> // NOLINT(misc-include-cleaner)
#include <GLFW/glfw3.h> // NOLINT(misc-include-cleaner)
#include <volk.h> // NOLINT(misc-include-cleaner)
#include <vulkan/vk_enum_string_helper.h>

#include <stdexcept>

namespace {

constexpr uint32_t height = 600; // NOLINT(misc-include-cleaner)
constexpr uint32_t width = 800; // NOLINT(misc-include-cleaner)

void setActiveExtensions(VkInstanceCreateInfo& createInfo) { // NOLINT(misc-include-cleaner)
	uint32_t glfwExtensionCount = 0; // NOLINT(misc-include-cleaner)
	char const* const* const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
}

void initVulkan(VkInstance* instance, Window::Version const& version) {
	VkResult const initResult = volkInitialize(); // NOLINT(misc-include-cleaner)
	if (VK_SUCCESS != initResult) { // NOLINT(misc-include-cleaner)
		fmt::print("Couldn't initialize Volk. Result code: {}\\n", // NOLINT(misc-include-cleaner)
				string_VkResult(initResult)); // NOLINT(misc-include-cleaner)
		throw std::runtime_error("Couldn't initialize Volk.");
	}

	VkApplicationInfo appInfo{}; // NOLINT(misc-include-cleaner)
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // NOLINT(misc-include-cleaner)
	appInfo.pApplicationName = "Vulkan Chess";
	appInfo.applicationVersion =
			VK_MAKE_VERSION(version.major, version.minor, version.patch); // NOLINT(misc-include-cleaner)
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // NOLINT(misc-include-cleaner)
	appInfo.apiVersion = VK_API_VERSION_1_0; // NOLINT(misc-include-cleaner)

	VkInstanceCreateInfo createInfo{}; // NOLINT(misc-include-cleaner)
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // NOLINT(misc-include-cleaner)
	createInfo.pApplicationInfo = &appInfo;
	setActiveExtensions(createInfo);
	createInfo.enabledLayerCount = 0;

	VkResult const createResult = vkCreateInstance(&createInfo, nullptr, instance); // NOLINT(misc-include-cleaner)
	if (VK_SUCCESS != createResult) { // NOLINT(misc-include-cleaner)
		fmt::print("Couldn't create Vulkan Instance. Result code: {}\\n", // NOLINT(misc-include-cleaner)
				string_VkResult(createResult)); // NOLINT(misc-include-cleaner)
		throw std::runtime_error("Couldn't create Vulkan Instance.");
	}
}

void initWindow(GLFWwindow*& window) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, "Vulkan Window", nullptr, nullptr);
}

} // namespace

Window::Window(Version const& version) {
	initVulkan(&instance, version);
	initWindow(window);
}

Window::~Window() {
	glfwDestroyWindow(window);

	glfwTerminate();
}

bool Window::keepOpen() {
	glfwPollEvents();
	return 0 == glfwWindowShouldClose(window);
}
