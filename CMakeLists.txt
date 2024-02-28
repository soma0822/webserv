cmake_minimum_required(VERSION 3.26)
project(webserv LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

set(PROGRAM webserv_test)
set(GTEST_VERSION 1.12.1)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-${GTEST_VERSION}.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/test/*_test.cpp")
add_executable(${PROGRAM}
        ${SOURCES}
        ${TEST_FILES}
)
target_link_libraries(${PROGRAM}
        gtest_main
)
file(GLOB_RECURSE INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/*")
foreach(DIR ${INCLUDE_DIRS})
    get_filename_component(DIR_PATH ${DIR} PATH)
    target_include_directories(${PROGRAM} PRIVATE ${DIR_PATH})
endforeach()
