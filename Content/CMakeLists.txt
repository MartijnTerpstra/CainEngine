cmake_minimum_required (VERSION 3.8.2)

project (Content CXX)

add_library(${PROJECT_NAME} INTERFACE)

add_library(Content::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
	INTERFACE "CONTENT_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}\""
	INTERFACE "SHADERS_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}\"")