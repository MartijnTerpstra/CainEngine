cmake_minimum_required (VERSION 3.8.2)

project (Graphics CXX)

file(GLOB SOURCELIST "*.cpp")

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Graphics::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	INTERFACE ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Core
	PUBLIC CainEngine::EntitySystem)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")