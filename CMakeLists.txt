cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

message(STATUS "Binary will be at: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Core source files
set(SRC_FILES
    src/menu.cpp
    src/intro.cpp
    src/animator.cpp
    src/resources.cpp
    src/dialogue.cpp
)

# Platform-specific main file
if(WIN32)
    list(APPEND SRC_FILES src/main_windows.cpp)
    set(WINDOWS TRUE)
elseif(UNIX)
    list(APPEND SRC_FILES src/main_linux.cpp)
endif()

# Create the executable
add_executable(main ${SRC_FILES} "src/LINUX.cpp")

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

# This makes sure your app is built as a GUI app on Windows
if(WINDOWS)
    set_target_properties(main PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
