cmake_minimum_required (VERSION 3.20)

project ("algorithms")

# Use C++20 standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include (FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable (
	algorithms 
	"algorithms.cpp" 
	"make_matrix.h" 
	"shortest_path.h"
)

target_link_libraries(algorithms gtest_main)
include(GoogleTest)
gtest_discover_tests(algorithms)