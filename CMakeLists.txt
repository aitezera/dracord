# Minimum CMake version required
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Project name
project(dracord VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the source and include directories
set(SRC_DIR src)
set(INCLUDE_DIR include)

# Find all .cpp and .hpp files in the specified directories
file(GLOB_RECURSE CPPFILES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HPPFILES "${INCLUDE_DIR}/*.hpp")

# Add include directories
include_directories(${INCLUDE_DIR})

# Add CPM to download any missing dependencies
include(cmake/CPM.cmake)

# Find required packages
CPMAddPackage(
  NAME SDL2pp
  GITHUB_REPOSITORY libSDL2pp/libSDL2pp
  GIT_TAG origin/master
)

CPMAddPackage(
  NAME jsoncpp
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.5
)

CPMAddPackage(
  NAME cpr
  GITHUB_REPOSITORY libcpr/cpr
  GIT_TAG 1.8.1
)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

# PNG and ZLIB seem to be broken using CPM
# Will have to mantually find and link them

# Add the executable target
add_executable(${PROJECT_NAME} ${CPPFILES})

target_link_libraries(${PROJECT_NAME}
    ZLIB::ZLIB
    PNG::PNG
    ${SDL2PP_LIBRARIES}
    jsoncpp_lib
    cpr::cpr
)

# Custom target for client
add_executable(client ${SRC_DIR}/main.cpp ${SRC_DIR}/window.cpp ${SRC_DIR}/ui.cpp ${SRC_DIR}/logging.cpp)

target_link_libraries(client
    ZLIB::ZLIB
    PNG::PNG
    ${SDL2PP_LIBRARIES}
    jsoncpp_lib
    cpr::cpr
)

# Custom target for requests
add_executable(requests ${SRC_DIR}/requests.cpp ${SRC_DIR}/logging.cpp ${SRC_DIR}/friend.cpp)

target_link_libraries(requests
    ZLIB::ZLIB
    PNG::PNG
    ${SDL2PP_LIBRARIES}
    jsoncpp_lib
    cpr::cpr
)