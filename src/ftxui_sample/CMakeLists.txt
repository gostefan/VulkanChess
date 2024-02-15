add_executable(intro main.cpp)

target_link_libraries(
  intro
  PRIVATE VulkanChess::VulkanChess_options
          VulkanChess::VulkanChess_warnings)

target_link_system_libraries(
  intro
  PRIVATE
          CLI11::CLI11
          fmt::fmt)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
