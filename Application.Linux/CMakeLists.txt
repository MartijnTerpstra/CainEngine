cmake_minimum_required (VERSION 3.25)

project (Application.Linux CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Linux)
