
# 
# External dependencies
# 

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glbinding REQUIRED)

find_package(OpenMP QUIET)


# 
# Executable name and options
# 

# Target name
set(target attributedvertexclouds-trajectories)

# Exit here if required dependencies are not met
message(STATUS "Example ${target}")

if (NOT OPENMP_FOUND)
    message("Loop parallelization in ${target} skipped: OpenMP not found")
endif()


# 
# Sources
# 

set(sources
    main.cpp
    
    TrajectoryRendering.h
    TrajectoryRendering.cpp
    
    TrajectoryNode.h
    TrajectoryNode.cpp
    
    TrajectoryVertexCloud.h
    TrajectoryVertexCloud.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


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
    ${DEFAULT_INCLUDE_DIRECTORIES}

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}

    glfw
    glm
    glbinding::glbinding
    glbinding::glbinding-aux
    common
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
    $<$<BOOL:${OPENMP_FOUND}>:USE_OPENMP>
    GLFW_INCLUDE_NONE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
    $<$<BOOL:${OPENMP_FOUND}>:${OpenMP_CXX_FLAGS}>
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:$<$<BOOL:"${OPENMP_FOUND}">:${OpenMP_CXX_FLAGS}>>
    ${DEFAULT_LINKER_OPTIONS}
)


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT examples
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT examples
)
