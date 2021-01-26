cmake_minimum_required (VERSION 3.8.2)

project (Graphics.Vulkan CXX)

file(GLOB SOURCELIST "*.cpp")

message(STATUS "SOURCELIST: ${SOURCELIST}")

find_package(Vulkan)

if(Vulkan_FOUND)
message(STATUS "Vulkan encountered")
else()
message(FATAL_ERROR "Vulkan not installed")
endif()

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Graphics::Vulkan ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External)

if (WIN32)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE -DVK_USE_PLATFORM_WIN32_KHR=1)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Graphics::Core
	PUBLIC Platform::Win32
	PRIVATE Vulkan::Vulkan)

elseif(UNIX)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Graphics::Core
	PUBLIC Platform::Linux
	PRIVATE Vulkan::Vulkan)

endif()

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")