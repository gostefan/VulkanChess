include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(VulkanChess_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(VulkanChess_setup_options)
  option(VulkanChess_ENABLE_HARDENING "Enable hardening" ON)
  option(VulkanChess_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    VulkanChess_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    VulkanChess_ENABLE_HARDENING
    OFF)

  VulkanChess_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR VulkanChess_PACKAGING_MAINTAINER_MODE)
    option(VulkanChess_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(VulkanChess_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(VulkanChess_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(VulkanChess_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(VulkanChess_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(VulkanChess_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(VulkanChess_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(VulkanChess_ENABLE_PCH "Enable precompiled headers" OFF)
    option(VulkanChess_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(VulkanChess_ENABLE_IPO "Enable IPO/LTO" ON)
    option(VulkanChess_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(VulkanChess_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(VulkanChess_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(VulkanChess_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(VulkanChess_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(VulkanChess_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(VulkanChess_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(VulkanChess_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(VulkanChess_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(VulkanChess_ENABLE_PCH "Enable precompiled headers" OFF)
    option(VulkanChess_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      VulkanChess_ENABLE_IPO
      VulkanChess_WARNINGS_AS_ERRORS
      VulkanChess_ENABLE_USER_LINKER
      VulkanChess_ENABLE_SANITIZER_ADDRESS
      VulkanChess_ENABLE_SANITIZER_LEAK
      VulkanChess_ENABLE_SANITIZER_UNDEFINED
      VulkanChess_ENABLE_SANITIZER_THREAD
      VulkanChess_ENABLE_SANITIZER_MEMORY
      VulkanChess_ENABLE_UNITY_BUILD
      VulkanChess_ENABLE_CLANG_TIDY
      VulkanChess_ENABLE_CPPCHECK
      VulkanChess_ENABLE_COVERAGE
      VulkanChess_ENABLE_PCH
      VulkanChess_ENABLE_CACHE)
  endif()

  VulkanChess_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (VulkanChess_ENABLE_SANITIZER_ADDRESS OR VulkanChess_ENABLE_SANITIZER_THREAD OR VulkanChess_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(VulkanChess_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(VulkanChess_global_options)
  if(VulkanChess_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    VulkanChess_enable_ipo()
  endif()

  VulkanChess_supports_sanitizers()

  if(VulkanChess_ENABLE_HARDENING AND VulkanChess_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR VulkanChess_ENABLE_SANITIZER_UNDEFINED
       OR VulkanChess_ENABLE_SANITIZER_ADDRESS
       OR VulkanChess_ENABLE_SANITIZER_THREAD
       OR VulkanChess_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${VulkanChess_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${VulkanChess_ENABLE_SANITIZER_UNDEFINED}")
    VulkanChess_enable_hardening(VulkanChess_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(VulkanChess_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(VulkanChess_warnings INTERFACE)
  add_library(VulkanChess_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  VulkanChess_set_project_warnings(
    VulkanChess_warnings
    ${VulkanChess_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(VulkanChess_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(VulkanChess_options)
  endif()

  include(cmake/Sanitizers.cmake)
  VulkanChess_enable_sanitizers(
    VulkanChess_options
    ${VulkanChess_ENABLE_SANITIZER_ADDRESS}
    ${VulkanChess_ENABLE_SANITIZER_LEAK}
    ${VulkanChess_ENABLE_SANITIZER_UNDEFINED}
    ${VulkanChess_ENABLE_SANITIZER_THREAD}
    ${VulkanChess_ENABLE_SANITIZER_MEMORY})

  set_target_properties(VulkanChess_options PROPERTIES UNITY_BUILD ${VulkanChess_ENABLE_UNITY_BUILD})

  if(VulkanChess_ENABLE_PCH)
    target_precompile_headers(
      VulkanChess_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(VulkanChess_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    VulkanChess_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(VulkanChess_ENABLE_CLANG_TIDY)
    VulkanChess_enable_clang_tidy(VulkanChess_options ${VulkanChess_WARNINGS_AS_ERRORS})
  endif()

  if(VulkanChess_ENABLE_CPPCHECK)
    VulkanChess_enable_cppcheck(${VulkanChess_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(VulkanChess_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    VulkanChess_enable_coverage(VulkanChess_options)
  endif()

  if(VulkanChess_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(VulkanChess_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(VulkanChess_ENABLE_HARDENING AND NOT VulkanChess_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR VulkanChess_ENABLE_SANITIZER_UNDEFINED
       OR VulkanChess_ENABLE_SANITIZER_ADDRESS
       OR VulkanChess_ENABLE_SANITIZER_THREAD
       OR VulkanChess_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    VulkanChess_enable_hardening(VulkanChess_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
