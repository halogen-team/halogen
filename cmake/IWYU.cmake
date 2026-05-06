# Function to generate source files for headers without matching sources
function(halogen_generate_iwyu_sources TARGET_NAME)
    set(generated_sources)
    set(remaining_sources ${SOURCES})
    foreach(header ${HEADERS})
        # Get the header name without path and extension
        get_filename_component(header_name ${header} NAME_WE)
        # Check if there's a matching source file
        set(has_source FALSE)
        foreach(source ${remaining_sources})
            get_filename_component(source_name ${source} NAME_WE)
            if("${source_name}" STREQUAL "${header_name}")
                set(has_source TRUE)
                list(REMOVE_ITEM remaining_sources ${source})
                break()
            endif()
        endforeach()

        # If no matching source, generate one
        if(NOT has_source)
            set(generated_file "${CMAKE_BINARY_DIR}/${TARGET_NAME}/iwyu_generated_sources/${header_name}.cpp")
            # Remove 'include/' prefix if present
            string(REGEX REPLACE "^include/" "" header_relative "${header}")
            file(WRITE ${generated_file} "#include \"${header_relative}\"\n")
            list(APPEND generated_sources ${generated_file})
        endif()
    endforeach()
    set(SOURCES ${SOURCES} ${generated_sources} PARENT_SCOPE)
endfunction()


function(halogen_setup_iwyu)
    find_program(INCLUDE_WHAT_YOU_USE_EXECUTABLE include-what-you-use)
    if(INCLUDE_WHAT_YOU_USE_EXECUTABLE)
        set(INCLUDE_WHAT_YOU_USE_ARGS ";-Xiwyu;--mapping_file=${CMAKE_CURRENT_SOURCE_DIR}/iwyu.imp;-Xiwyu;--no_fwd_decls")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE_EXECUTABLE} ${INCLUDE_WHAT_YOU_USE_ARGS})
    else()
        message(FATAL_ERROR "include-what-you-use requested but executable not found")
        return()
    endif()
endfunction()