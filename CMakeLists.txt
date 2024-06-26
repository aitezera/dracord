# Minimum CMake version required
cmake_minimum_required(VERSION 3.10...3.18)

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

# Find required packages
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2PP REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

# Include SDL2PP Directory
include_directories(${SDL2PP_INCLUDE_DIRS})

# Add include directories for found packages
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIRS})
include_directories(${CPR_INCLUDE_DIRS})

# Add the executable target
add_executable(${PROJECT_NAME} ${CPPFILES})

# Link the required libraries
target_link_libraries(${PROJECT_NAME}
    ZLIB::ZLIB
    PNG::PNG
    ${SDL2PP_LIBRARIES}
    jsoncpp_lib
    cpr::cpr
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Possibily dont need /usr/include/SDL2 anymore 
else()
    message(FATAL_ERROR "Your OS is not supported by this CMake configuration")
endif()
 
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

# Custom clean target
#add_custom_target(clean
#    COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_NAME} client requests
#    COMMENT "Cleaning up generated files"
#)