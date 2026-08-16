cmake_minimum_required (VERSION 3.25)

project (CommonTests CXX)

if (BUILD_TESTING)

file(GLOB SOURCELIST "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCELIST})

target_link_libraries(${PROJECT_NAME}
	PRIVATE Common::Common
	PRIVATE Common::TestSupport
	PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

endif()
