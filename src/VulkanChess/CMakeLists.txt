add_executable(VulkanChess main.cpp Window.cpp)

target_link_libraries(
  VulkanChess
  PRIVATE VulkanChess::VulkanChess_options
          VulkanChess::VulkanChess_warnings)

target_link_system_libraries(
  VulkanChess
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          glfw
          glm::glm-header-only
          Vulkan::Headers
		  Vulkan::UtilityHeaders
          volk::volk)

target_include_directories(VulkanChess PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
