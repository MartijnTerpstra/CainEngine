cmake_minimum_required (VERSION 3.8.2)

project (Platform CXX)

add_library(${PROJECT_NAME} INTERFACE)

add_library(Platform::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	INTERFACE ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	INTERFACE Common::Common)