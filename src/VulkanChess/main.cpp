#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define VK_NO_PROTOTYPES

#include <CLI/CLI.hpp> // NOLINT(misc-include-cleaner)
#include <fmt/format.h> // NOLINT(misc-include-cleaner)
#include <volk.h> // NOLINT(misc-include-cleaner)

#include <cstdlib>
#include <exception>
#include <optional>
#include <string>

#include "Window.h"

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `VulkanChess`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

class VulkanChessApp final {
  public:
	VulkanChessApp() = default;

	static void run(bool showExtensions) {
		Window window{ Window::Version{ VulkanChess::cmake::project_version_major,
				VulkanChess::cmake::project_version_minor,
				VulkanChess::cmake::project_version_patch } };
		if (showExtensions) {
			printAvailableExtensions();
		}
		mainLoop(window);
	}

  private:
	static void printAvailableExtensions() {
		uint32_t extensionCount = 0; // NOLINT(misc-include-cleaner)
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount); // NOLINT(misc-include-cleaner)
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		fmt::println("Available Extensions:"); // NOLINT(misc-include-cleaner)
		for (auto const& extension : extensions) {
			fmt::println("  {0}", extension.extensionName); // NOLINT(misc-include-cleaner)
		}
	}

	static void mainLoop(Window& window) {
		while (window.keepOpen()) {
			// Main Loop goes here
		}
	}
};

int main(int argc, const char** argv) {
	try {
		std::string const appName = fmt::format("{} version {}", // NOLINT(misc-include-cleaner)
				VulkanChess::cmake::project_name,
				VulkanChess::cmake::project_version);
		CLI::App app{ appName }; // NOLINT(misc-include-cleaner)

		std::optional<std::string> message;
		app.add_option("-m,--message", message, "A message to print back out");
		bool show_version = false; // NOLINT(misc-const-correctness)
		app.add_flag("--version", show_version, "Show version information");
		bool show_extensions = false; // NOLINT(misc-const-correctness)
		app.add_flag("--printExtensions", show_extensions, "Prints available Vulkan extensions to the console.");

		CLI11_PARSE(app, argc, argv); // NOLINT(misc-include-cleaner)

		if (show_version) {
			fmt::print("{}\n", VulkanChess::cmake::project_version); // NOLINT(misc-include-cleaner)
			return EXIT_SUCCESS;
		}

		VulkanChessApp::run(show_extensions);

		return EXIT_SUCCESS;
	} catch (const std::exception& e) {
		fmt::print("Unhandled exception in main: {}\n", e.what()); // NOLINT(misc-include-cleaner)
		return EXIT_FAILURE;
	}
}
