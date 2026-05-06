# Function to apply compile options to a specific target
function(halogen_apply_compile_options TARGET_NAME)
    # General compile options
    target_compile_options(${TARGET_NAME} PRIVATE
        -pedantic                   # Warn on language extensions
        -Wall -Wextra               # reasonable and standard
        -Wshadow                    # warn the user if a variable declaration shadows one from a parent context
        -Wnon-virtual-dtor          # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
        -Wold-style-cast            # warn for c-style casts
        -Wcast-align                # warn for potential performance problem casts
        -Wunused                    # warn on anything being unused
        -Woverloaded-virtual        # warn if you overload (not override) a virtual function
        -Wpedantic                  # warn if non-standard C++ is used
        -Wconversion                # warn on type conversions that may lose data
        -Wsign-conversion           # warn on sign conversions
        -Wmisleading-indentation    # warn if indentation implies blocks where blocks do not exist
        -Wnull-dereference          # warn if a null dereference is detected
        -Wdouble-promotion          # warn if float is implicitly promoted to double
        -Wformat=2                  # warn on security issues around functions that format output (i.e., printf)
        -Werror                     # Make all warnings into errors
    )
    
    # GCC-specific compile options

    message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${TARGET_NAME} PRIVATE
            -Wduplicated-cond # warn if if / else chain has duplicated conditions
            -Wduplicated-branches # warn if if / else branches have duplicated code
            -Wlogical-op # logical operations being used where bitwise were probably wanted
            -Wuseless-cast # warn if you perform a cast to the same type
        )
    endif()
endfunction()