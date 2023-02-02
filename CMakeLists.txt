# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(symsolbin CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------

option(SYMSOLBIN_BUILD_EXAMPLES "Build examples" OFF)
option(SYMSOLBIN_STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(SYMSOLBIN_WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)

# -----------------------------------------------------------------------------
# MODULE PATH
# -----------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

# -----------------------------------------------------------------------------
# LIBRARIES
# -----------------------------------------------------------------------------

# Find GiNaC.
find_package(GiNaC REQUIRED)

# -----------------------------------------------------------------------------
# COMPILATION FLAGS
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings that suggest using MSVC-specific safe functions
    set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} -D_CRT_SECURE_NO_WARNINGS)

    if(SYMSOLBIN_WARNINGS_AS_ERRORS)
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /WX)
    endif(SYMSOLBIN_WARNINGS_AS_ERRORS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(SYMSOLBIN_WARNINGS_AS_ERRORS)
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} -Werror)
    endif(SYMSOLBIN_WARNINGS_AS_ERRORS)
endif()

if(SYMSOLBIN_STRICT_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Mark system headers as external for MSVC explicitly
        # https://devblogs.microsoft.com/cppblog/broken-warnings-theory
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /experimental:external)
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /external:I ${CMAKE_BINARY_DIR})
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /external:anglebrackets)
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /external:W0)
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} /W4)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SYMSOLBIN_COMPILE_OPTIONS ${SYMSOLBIN_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -pedantic)
    endif()
endif(SYMSOLBIN_STRICT_WARNINGS)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ library.
add_library(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/symsolbin/solver/analog_model.cpp
    ${PROJECT_SOURCE_DIR}/src/symsolbin/solver/classifier.cpp
    ${PROJECT_SOURCE_DIR}/src/symsolbin/structure/edge.cpp
    ${PROJECT_SOURCE_DIR}/src/symsolbin/structure/node.cpp
    ${PROJECT_SOURCE_DIR}/src/symsolbin/structure/value.cpp
    ${PROJECT_SOURCE_DIR}/src/symsolbin/model/model_gen.cpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
# Inlcude header directories.
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
# Set compilation flags.
target_compile_options(${PROJECT_NAME} PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
# Set linking flags.
target_link_libraries(${PROJECT_NAME} PUBLIC ${GINAC_LIBRARIES} dl)

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(SYMSOLBIN_BUILD_EXAMPLES)

    # Add the example.
    add_executable(${PROJECT_NAME}_double_rlc ${PROJECT_SOURCE_DIR}/examples/double_rlc.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_double_rlc PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_double_rlc PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_double_rlc PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_double_rlc PUBLIC cxx_std_17)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_diode ${PROJECT_SOURCE_DIR}/examples/diode.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_diode PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_diode PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_diode PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_diode PUBLIC cxx_std_17)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_memristor ${PROJECT_SOURCE_DIR}/examples/memristor.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_memristor PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_memristor PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_memristor PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_memristor PUBLIC cxx_std_17)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_not ${PROJECT_SOURCE_DIR}/examples/not.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_not PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_not PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_not PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_not PUBLIC cxx_std_17)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_rc ${PROJECT_SOURCE_DIR}/examples/rc.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_rc PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_rc PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_rc PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_rc PUBLIC cxx_std_17)
    
    # Add the example.
    add_executable(${PROJECT_NAME}_rlc ${PROJECT_SOURCE_DIR}/examples/rlc.cpp)
    # Set compilation flags.
    target_compile_options(${PROJECT_NAME}_rlc PUBLIC ${SYMSOLBIN_COMPILE_OPTIONS})
    # Inlcude header directories.
    target_include_directories(${PROJECT_NAME}_rlc PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Set the linked libraries.
    target_link_libraries(${PROJECT_NAME}_rlc PUBLIC ${PROJECT_NAME})
    # Set compiler flags.
    target_compile_features(${PROJECT_NAME}_rlc PUBLIC cxx_std_17)
    
endif(SYMSOLBIN_BUILD_EXAMPLES)

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

find_package(Doxygen)

if(DOXYGEN_FOUND)
    
    message(STATUS "Retrieving `doxygen-awesome-css`...")

    # = RETIVAL ===============================================================
    # Include fetch content.
    include(FetchContent)
    # Record the options that describe how to populate the specified content.
    FetchContent_Declare(
        doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG 4cd62308d825fe0396d2f66ffbab45d0e247724c # 2.0.3
    )
    # Retrieve the properties related to the content.
    FetchContent_GetProperties(doxygenawesome)
    # If not populated, make the content available.
    if(NOT doxygenawesome_POPULATED)
        # Ensures the named dependencies have been populated.
        FetchContent_MakeAvailable(doxygenawesome)
        # Hide fetchcontent variables, otherwise with ccmake it's a mess.
        mark_as_advanced(FORCE
            FETCHCONTENT_QUIET FETCHCONTENT_BASE_DIR FETCHCONTENT_FULLY_DISCONNECTED FETCHCONTENT_UPDATES_DISCONNECTED
            FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
        )
    endif()

    # = CUSTOMIZATION =========================================================
    set(DOXYGEN_PROJECT_NAME "Symsolbin Library")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_WARN_FORMAT "$file:$line: $text")
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )
    doxygen_add_docs(
        ${PROJECT_NAME}_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_SOURCE_DIR}/include/symsolbin/simulation/simulation.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/simulation/double_op.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/simulation/analog_pair.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/structure/node.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/structure/value.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/structure/edge.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/solver/analog_model.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/solver/classifier.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/solver/name_generator.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/solver/ginac_helper.hpp
        ${PROJECT_SOURCE_DIR}/include/symsolbin/model/model_gen.hpp
    )
endif()
