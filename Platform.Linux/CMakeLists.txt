cmake_minimum_required (VERSION 3.8.2)

project (Platform.Linux CXX)

file(GLOB SOURCELIST "*.cpp")

find_package(X11 REQUIRED)

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Platform::Linux ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External
	PUBLIC ${X11_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Core
	PUBLIC ${X11_LIBRARIES})

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")