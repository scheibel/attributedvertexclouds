
# 
# External dependencies
# 

find_package(GLM REQUIRED)
find_package(glbinding REQUIRED)

# 
# Library name and options
# 

# Target name
set(target common)

# Exit here if required dependencies are not met
message(STATUS "Helper ${target}")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")

set(headers
    ${include_path}/common.h
)

set(sources
    ${source_path}/common.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.inl$" 
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.inl$" 
    ${source_group} ${sources})


# 
# Create library
# 

# since we use stl and stl is intended to use exceptions, exceptions should not be disabled
#if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    # workaround for removing default flags 
    # string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) 
#endif ()


# Build library
add_library(${target}
    STATIC
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLM_INCLUDE_DIR}

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    glbinding::glbinding

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    
    PUBLIC
    ${DEFAULT_COMPILE_DEFINITIONS}
    GLM_FORCE_RADIANS

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)
