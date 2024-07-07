#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#define VK_NO_PROTOTYPES

#include "Window.h"

#include <fmt/format.h> // NOLINT(misc-include-cleaner)
#include <GLFW/glfw3.h> // NOLINT(misc-include-cleaner)
#include <volk.h> // NOLINT(misc-include-cleaner)
#include <vulkan/vk_enum_string_helper.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace {

constexpr uint32_t height = 600; // NOLINT(misc-include-cleaner)
constexpr uint32_t width = 800; // NOLINT(misc-include-cleaner)

constexpr bool ENABLE_VALIDATION_LAYERS = true;
constexpr std::array<char const*, 1> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

void setActiveExtensions(VkInstanceCreateInfo& createInfo) { // NOLINT(misc-include-cleaner)
	uint32_t glfwExtensionCount = 0; // NOLINT(misc-include-cleaner)
	char const* const* const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
}

std::vector<VkLayerProperties> getValidationLayers() { // NOLINT(misc-include-cleaner)
	uint32_t layerCount = 0; // NOLINT(misc-include-cleaner)
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount); // NOLINT(misc-include-cleaner)
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	return availableLayers;
}

void checkValidationLayerSupport(bool printAvailableLayers) {
	std::vector<VkLayerProperties> const availableLayers = getValidationLayers(); // NOLINT(misc-include-cleaner)

	if (printAvailableLayers) {
		fmt::print("Available Validation Layers\n"); // NOLINT(misc-include-cleaner)
		for (VkLayerProperties const& availableLayer : availableLayers) { // NOLINT(misc-include-cleaner)
			fmt::print("  {}\n", availableLayer.layerName); // NOLINT(misc-include-cleaner)
		}
	}

	for (char const* requestedLayer : VALIDATION_LAYERS) {
		if (!std::any_of(availableLayers.begin(),
					availableLayers.end(),
					[&requestedLayer](VkLayerProperties const& availableLayer) { // NOLINT(misc-include-cleaner)
						return std::strcmp(requestedLayer, static_cast<char const*>(availableLayer.layerName)) == 0;
					})) {
			throw std::runtime_error{ fmt::format(
					"Requested validation layer '{}' not available.", requestedLayer) }; // NOLINT(misc-include-cleaner)
		}
	}
}

void initVulkan(VkInstance* instance, Window::Version const& version) {
	VkResult const initResult = volkInitialize(); // NOLINT(misc-include-cleaner)
	if (VK_SUCCESS != initResult) { // NOLINT(misc-include-cleaner)
		fmt::print("Couldn't initialize Volk. Result code: {}\n", // NOLINT(misc-include-cleaner)
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

	if constexpr (ENABLE_VALIDATION_LAYERS) {
		checkValidationLayerSupport(true);
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size()); // NOLINT(misc-include-cleaner)
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}

	VkResult const createResult = vkCreateInstance(&createInfo, nullptr, instance); // NOLINT(misc-include-cleaner)
	if (VK_SUCCESS != createResult) { // NOLINT(misc-include-cleaner)
		fmt::print("Couldn't create Vulkan Instance. Result code: {}\n", // NOLINT(misc-include-cleaner)
				string_VkResult(createResult)); // NOLINT(misc-include-cleaner)
		throw std::runtime_error("Couldn't create Vulkan Instance.");
	}
	volkLoadInstance(*instance);
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
	vkDestroyInstance(instance, nullptr);
}

bool Window::keepOpen() {
	glfwPollEvents();
	return 0 == glfwWindowShouldClose(window);
}
