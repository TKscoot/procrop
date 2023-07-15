cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME procrop)
project(${PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/CMakeFiles")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/source ${PROJECT_SOURCE_DIR}/vendor)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/source/*.cpp)

file(GLOB helloworld_SRC
     "*.h"
     "*.cpp"
)
set(CMAKE_BUILD_TYPE Debug)
# executable & links
add_executable(${PROJECT_NAME} ${SRC_FILES} )
target_link_libraries(procrop PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} spdlog::spdlog)

#add_custom_command(OUTPUT)

add_custom_target(run
    COMMAND ./${PROJECT_NAME}
	VERBATIM)
