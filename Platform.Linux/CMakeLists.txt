cmake_minimum_required (VERSION 3.25)

project (Platform.Linux CXX)

file(GLOB SOURCELIST "*.cpp")

find_package(X11 REQUIRED)

if(NOT X11_Xrandr_FOUND)
	message(FATAL_ERROR "Platform.Linux requires the Xrandr X11 extension (libxrandr-dev)")
endif()

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Platform::Linux ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External
	PUBLIC ${X11_INCLUDE_DIR}
	PUBLIC ${X11_Xrandr_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Core
	PUBLIC ${X11_LIBRARIES}
	PUBLIC ${X11_Xrandr_LIB})

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")