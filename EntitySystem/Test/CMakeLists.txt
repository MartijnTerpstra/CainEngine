cmake_minimum_required (VERSION 3.8.2)

project (EntitySystemTests CXX)

if (BUILD_TESTING)

file(GLOB SOURCELIST "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCELIST})

target_link_libraries(${PROJECT_NAME}
	PRIVATE CainEngine::EntitySystem
	PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

endif()
