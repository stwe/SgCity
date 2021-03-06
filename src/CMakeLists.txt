cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES
        "*.h"
        "*.cpp"
        )

include(../conanbuildinfo.cmake)
conan_basic_setup()

add_executable(SgCity ${SRC_FILES})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("-- USE DEBUG SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC SG_CITY_DEBUG_BUILD GLFW_INCLUDE_NONE)
else()
    message("-- USE RELEASE SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
