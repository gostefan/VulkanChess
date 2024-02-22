include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(VulkanChess_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#10.2.1")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.3.2")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.3.2")
  endif()

  if(NOT TARGET glfw)
	CPMAddPackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.9
        OPTIONS
          "GLFW_BULID_DOCS OFF"
          "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BUILD_TESTS OFF"
          "GLFW_INSTALL OFF")
  endif()

  if(NOT TARGET glm::glm-header-only)
    cpmaddpackage("gh:g-truc/glm#1.0.0")
  endif()

  if(NOT TARGET Vulkan::Headers)
    cpmaddpackage("gh:KhronosGroup/Vulkan-Headers@1.3.275")
  endif()

  if(NOT TARGET volk::volk)
    cpmaddpackage("gh:zeux/volk#vulkan-sdk-1.3.275.0")
    get_target_property(VULKAN_HEADERS_DIR Vulkan::Headers INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(volk PUBLIC "${VULKAN_HEADERS_DIR}")
  endif()

endfunction()
