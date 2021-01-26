cmake_minimum_required (VERSION 3.8.2)

project (Editor CXX)

file(GLOB SOURCELIST "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCELIST})

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Win32
	PUBLIC Graphics::DX11
	PRIVATE Content::Content
	PRIVATE Editor::ShaderCompiler)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")