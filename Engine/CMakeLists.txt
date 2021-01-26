cmake_minimum_required (VERSION 3.8.2)

project (Engine CXX)

file(GLOB SOURCELIST "*.cpp")

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(CainEngine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Core
	PUBLIC Graphics::Core
	PUBLIC EntitySystem)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")