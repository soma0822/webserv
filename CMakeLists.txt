cmake_minimum_required(VERSION 3.26)
project(webserv LANGUAGES CXX)

# プロジェクトの設定
set(PROGRAM webserv_test)
set(GTEST_VERSION 1.12.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Testのインストール
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-${GTEST_VERSION}.zip
)
FetchContent_MakeAvailable(googletest)

# ソースファイルの取得
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/test/*_test.cpp")

# テストのビルド
add_executable(${PROGRAM}
        ${SOURCES}
        ${TEST_FILES}
)
add_compile_options(-Wall -Wextra -Werror)

target_link_libraries(${PROGRAM}
        gtest_main
)
file(GLOB_RECURSE INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/*")
foreach (DIR ${INCLUDE_DIRS})
    get_filename_component(DIR_PATH ${DIR} PATH)
    target_include_directories(${PROGRAM} PRIVATE ${DIR_PATH})
endforeach ()

# FakeItをインクルード
target_include_directories(${PROGRAM} PRIVATE "${PROJECT_SOURCE_DIR}/lib/fakeit")
