cmake_minimum_required(VERSION 3.25.1)
set(CMAKE_CXX_STANDARD 23)

project(halogen LANGUAGES CXX)

add_compile_options(
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O3>
    
    -pedantic # Warn on language extensions
    -Wall -Wextra # reasonable and standard
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicitly promoted to double
    -Wformat=2 # warn on security issues around functions that format output (i.e., printf)
    -Weverything # warn on everything
    -Werror # Make all warnings into errors
)

# GCC-specific compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # logical operations being used where bitwise were probably wanted
        -Wuseless-cast # warn if you perform a cast to the same type
    )
endif()

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/IWYU.cmake")

# Update all submodules
execute_process(
    COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Conan integration
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    message(STATUS "Conan: creating build")
    execute_process(COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} --output-folder=${CMAKE_BINARY_DIR} --build=missing -s build_type=${CMAKE_BUILD_TYPE}
    RESULT_VARIABLE return_code)
    if(NOT ${return_code} EQUAL "0")
        message(FATAL_ERROR "Conan install failed '${return_code}'")
    endif()
endif()

if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUserPresets.json")
    file(REMOVE "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUserPresets.json")
endif()

set(LIB_COMMON_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Option to enable or disable include-what-you-use
option(USE_IWYU "Enable include-what-you-use" OFF)
if(USE_IWYU)
    find_program(INCLUDE_WHAT_YOU_USE_EXECUTABLE include-what-you-use)
    if(INCLUDE_WHAT_YOU_USE_EXECUTABLE)
        set(INCLUDE_WHAT_YOU_USE_ARGS ";-Xiwyu;--mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/iwyu.imp;-Xiwyu;--no_fwd_decls")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE_EXECUTABLE} ${INCLUDE_WHAT_YOU_USE_ARGS})
    else()
        message(FATAL_ERROR "include-what-you-use requested but executable not found")
        return()
    endif()
endif()

add_subdirectory(libraries)