cmake_minimum_required(VERSION 3.20)
project(calorie_counter_backend LANGUAGES CXX)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Building with coverage instrumentation")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage only supported with GCC/Clang")
    endif()
endif()
# Set C++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings (good for dev)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -ftemplate-backtrace-limit=0 -fdiagnostics-color=always)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Option to build tests
option(BUILD_TESTS "Build unit and integration tests" OFF)


# put executables in build/bin and libs in build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectories
add_subdirectory(src)
# Only enable tests when requested
if (BUILD_TESTS)
  include(CTest)
  set(CMAKE_CTEST_ARGUMENTS "--output-on-failure;--verbose")
  enable_testing()
  add_subdirectory(tests)
endif()

#############clang-format#############
# ---- clang-format target ----
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
  file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.[ch]pp
    ${CMAKE_SOURCE_DIR}/src/*.[ch]
    ${CMAKE_SOURCE_DIR}/tests/*.[ch]pp
    ${CMAKE_SOURCE_DIR}/tests/*.[ch]
  )

  list(LENGTH ALL_CXX_SOURCE_FILES COUNT)
  message(STATUS "clang-format will run on ${COUNT} files")

  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
    COMMENT "Running clang-format"
  )
endif()

