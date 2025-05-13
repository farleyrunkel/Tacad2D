# Define source files for enumFlagsLib (header-only library)
set(ENUMFLAGS_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/enumflags/flags.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/enumflags/flagsfwd.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/enumflags/iterator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/enumflags/allow_flags.hpp"
)

# Create an interface library for enumFlags
add_library(enumFlags INTERFACE)
target_include_directories(enumFlags INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Define source files for sketchSolveLib
set(SKETCHSOLVE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/sketchsolve/solve.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/sketchsolve/solve.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sketchsolve/solveimpl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sketchsolve/errorfuncs.cpp"
)

# Create a static library named sketchSolveLib
add_library(sketchSolve STATIC ${SKETCHSOLVE_SOURCES})
# Specify include directories for sketchSolveLib
target_include_directories(sketchSolve PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
