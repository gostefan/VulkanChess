add_executable(VulkanChess main.cpp)

target_link_libraries(
  VulkanChess
  PRIVATE VulkanChess::VulkanChess_options
          VulkanChess::VulkanChess_warnings)

target_link_system_libraries(
  VulkanChess
  PRIVATE
          CLI11::CLI11
          fmt::fmt)

target_include_directories(VulkanChess PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
