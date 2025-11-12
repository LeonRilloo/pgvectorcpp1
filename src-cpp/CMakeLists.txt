# CMake configuration for pgvectorcpp library

cmake_minimum_required(VERSION 3.10)
project(pgvectorcpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/indexes
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/postgres
)

# Source files (to be populated as implementation progresses)
set(CORE_SOURCES
    core/vector_core.cpp
)

set(INDEXES_SOURCES
    indexes/vector_indexes.cpp
)

set(UTILS_SOURCES
    utils/vector_utils.cpp
)

set(IO_SOURCES
    io/vector_io.cpp
)

set(POSTGRES_SOURCES
    postgres/pg_integration.cpp
)

# Create library
add_library(pgvectorcpp STATIC
    ${CORE_SOURCES}
    ${INDEXES_SOURCES}
    ${UTILS_SOURCES}
    ${IO_SOURCES}
    ${POSTGRES_SOURCES}
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
