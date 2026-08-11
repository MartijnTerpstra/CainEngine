cmake_minimum_required (VERSION 3.8.2)

project (CommonTestSupport CXX)

if (BUILD_TESTING)

file(GLOB SOURCELIST "*.cpp")

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Common::TestSupport ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC gtest)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

endif()
