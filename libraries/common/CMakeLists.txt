set(HEADERS
    include/common/classmacros.hpp
    include/common/treenode.hpp
)

set(SOURCES
)

halogen_add_library(halogen-common ${HEADERS} ${SOURCES})

target_include_directories(halogen-common PUBLIC include)

if(DEBUG)
    halogen_apply_compile_options(halogen-common)
endif()