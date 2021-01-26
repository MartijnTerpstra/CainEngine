cmake_minimum_required (VERSION 3.8.2)

project (Graphics.DX12 CXX)

file(GLOB SOURCELIST "*.cpp")

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Graphics::DX12 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Graphics::Core
	PUBLIC Platform::Win32
	PRIVATE d3d12.lib
	PRIVATE dxgi.lib
	PRIVATE d3dcompiler.lib)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")