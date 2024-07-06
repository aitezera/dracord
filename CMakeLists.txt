# Minimum CMake version required
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Project name
project(dracord VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the source and include directories
set(SRC_DIR src)
set(INCLUDE_DIR include)

# Find all .cpp and .hpp files in the specified directories
file(GLOB_RECURSE CPPFILES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HPPFILES "${INCLUDE_DIR}/*.hpp")

# Add include directories
include_directories(${INCLUDE_DIR})

include(cmake/CPM.cmake)

# PNG and ZLIB seem to be broken using CPM
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

#CPMAddPackage(
#    NAME ZLIB
#    GITHUB_REPOSITORY madler/zlib
#    GIT_TAG v1.3.1
#)

#CPMAddPackage(
#    NAME PNG
#    GITHUB_REPOSITORY glennrp/libpng
#    GIT_TAG v1.6.43
#)

CPMAddPackage(
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.30.5
)

CPMAddPackage(
    NAME SDL2_IMAGE
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG release-2.8.2
)

CPMAddPackage(
    NAME SDL2_TTF
    GITHUB_REPOSITORY libsdl-org/SDL_ttf
    GIT_TAG release-2.22.0
)

CPMAddPackage(
    NAME jsoncpp
    GITHUB_REPOSITORY open-source-parsers/jsoncpp
    GIT_TAG 1.9.5
)

CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.10.5

)

CPMAddPackage(
    NAME ixwebsocket
    GITHUB_REPOSITORY machinezone/IXWebSocket
    GIT_TAG v11.4.5
)

# Add the executable target
add_executable(${PROJECT_NAME} ${CPPFILES})

target_link_libraries(${PROJECT_NAME}
    ZLIB::ZLIB
    PNG::PNG
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
    jsoncpp_lib
    cpr::cpr
    ixwebsocket::ixwebsocket
)

# Custom target for client
add_executable(client ${SRC_DIR}/main.cpp ${SRC_DIR}/window.cpp ${SRC_DIR}/label.cpp ${SRC_DIR}/logging.cpp ${SRC_DIR}/button.cpp ${SRC_DIR}/textfield.cpp)

target_link_libraries(client
    ZLIB::ZLIB
    PNG::PNG
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)

# Custom target for requests
add_executable(requests ${SRC_DIR}/requests.cpp ${SRC_DIR}/websocket.cpp ${SRC_DIR}/logging.cpp)

target_link_libraries(requests
    ZLIB::ZLIB
    PNG::PNG
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
    jsoncpp_lib
    cpr::cpr
    ixwebsocket::ixwebsocket
)