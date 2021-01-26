cmake_minimum_required (VERSION 3.8.2)

project (Application.Win32 CXX)

file(GLOB SOURCELIST "*.cpp")

add_executable(${PROJECT_NAME} main.cpp) #${SOURCELIST})

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Win32
	PUBLIC Graphics::DX11
	PUBLIC CainEngine::Engine
	PUBLIC EntitySystem
	PRIVATE Content::Content)