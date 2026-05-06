cmake_minimum_required(VERSION 3.25.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(halogen LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(compile-options)
include(build-utils)

include(conan)
# halogen_conan_install()

add_subdirectory(libraries)

if(USE_IWYU)
    include(iwyu)
    halogen_setup_iwyu()
endif()