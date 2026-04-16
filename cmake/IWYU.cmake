# Function to generate source files for headers without matching sources
function(generate_iwyu_sources TARGET_NAME)
    set(generated_sources)
    foreach(header ${HEADERS})
        # Get the header name without path and extension
        get_filename_component(header_name ${header} NAME_WE)
        # Check if there's a matching source file
        set(has_source FALSE)
        foreach(source ${SOURCES})
            get_filename_component(source_name ${source} NAME_WE)
            if(${source_name} STREQUAL ${header_name})
                set(has_source TRUE)
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
