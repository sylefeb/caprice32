cmake_minimum_required(VERSION 3.16)

project(cap32
    VERSION 1.0
    LANGUAGES CXX
)

option(WITHOUT_GL "Build without OpenGL support" OFF)

# Default build type (matches Makefile behavior)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------

set(SRCDIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(TSTDIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB SOURCES CONFIGURE_DEPENDS
    ${SRCDIR}/*.cpp
)

file(GLOB HEADERS CONFIGURE_DEPENDS
    ${SRCDIR}/*.h
)

file(GLOB_RECURSE SOURCES_IPF CONFIGURE_DEPENDS
    ${SRCDIR}/capsimg/*.cpp
)

file(GLOB_RECURSE HEADERS_IPF CONFIGURE_DEPENDS
    ${SRCDIR}/capsimg/*.cpp
)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${TSTDIR}/*.cpp
)

file(GLOB_RECURSE TEST_HEADERS CONFIGURE_DEPENDS
    ${TSTDIR}/*.h
)

# ------------------------------------------------------------------------------

add_executable(cap32
    main.cpp
    ${sources_core}
    ${SOURCES}
    ${HEADERS}
    ${HEADERS_IPF}
)

target_include_directories(cap32 PUBLIC
  ${SRCDIR}/
  ${SRCDIR}/capsimg/CAPSImg/
  ${SRCDIR}/capsimg/LibIPF/
  ${SRCDIR}/capsimg/Core/
  ${SRCDIR}/capsimg/Codec/
  ${SRCDIR}/capsimg/Device/
)

target_compile_definitions(cap32 PUBLIC APP_PATH="/")
target_compile_definitions(cap32 PRIVATE WITHOUT_GL)

target_compile_options(cap32 PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -funroll-loops
    -ffast-math
    -fomit-frame-pointer
    -finline-functions
)

# ------------------------------------------------------------------------------

add_custom_command(
    TARGET cap32
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary cap32 ${CMAKE_SOURCE_DIR}/bin/cap32.bin
    COMMENT "Creating cap32.bin"
)
add_custom_command(
    TARGET cap32
    POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} ARGS --disassemble cap32 > ${CMAKE_SOURCE_DIR}/bin/cap32.s
    COMMENT "Creating cap32.s"
)

# ------------------------------------------------------------------------------
