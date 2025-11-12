# CMake configuration for pgvectorcpp library

cmake_minimum_required(VERSION 3.10)
project(pgvectorcpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for optimization and warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /arch:AVX2")
endif()

# Find PostgreSQL
find_package(PostgreSQL REQUIRED)
if(NOT PostgreSQL_FOUND)
    message(FATAL_ERROR "PostgreSQL not found")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/indexes
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/postgres
    ${PostgreSQL_INCLUDE_DIRS}
    ${PostgreSQL_SERVER_INCLUDE_DIRS}
)

# Source files
set(CORE_SOURCES
    core/vector_core.cpp
    core/vector_core.h
)

set(INDEXES_SOURCES
    indexes/vector_indexes.cpp
    indexes/vector_indexes.h
)

set(UTILS_SOURCES
    utils/vector_utils.cpp
    utils/vector_utils.h
)

set(IO_SOURCES
    io/vector_io.cpp
    io/vector_io.h
)

set(POSTGRES_SOURCES
    postgres/pg_integration.cpp
    postgres/pg_integration.h
)

# Create library
add_library(pgvectorcpp STATIC
    ${CORE_SOURCES}
    ${INDEXES_SOURCES}
    ${UTILS_SOURCES}
    ${IO_SOURCES}
    ${POSTGRES_SOURCES}
)

# Link PostgreSQL libraries
target_link_libraries(pgvectorcpp
    ${PostgreSQL_LIBRARIES}
)

# Set additional properties
set_target_properties(pgvectorcpp PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# Installation rules
install(TARGETS pgvectorcpp
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY core/ DESTINATION include/pgvectorcpp/core FILES_MATCHING PATTERN "*.h")
install(DIRECTORY indexes/ DESTINATION include/pgvectorcpp/indexes FILES_MATCHING PATTERN "*.h")
install(DIRECTORY utils/ DESTINATION include/pgvectorcpp/utils FILES_MATCHING PATTERN "*.h")
install(DIRECTORY io/ DESTINATION include/pgvectorcpp/io FILES_MATCHING PATTERN "*.h")
install(DIRECTORY postgres/ DESTINATION include/pgvectorcpp/postgres FILES_MATCHING PATTERN "*.h")
