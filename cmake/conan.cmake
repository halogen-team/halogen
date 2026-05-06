function(halogen_conan_install)
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
endfunction()