cmake_minimum_required (VERSION 3.8.2)

project (Common CXX)

file(GLOB SOURCELIST "*.cpp")

add_library(${PROJECT_NAME} ${SOURCELIST})

add_library(Common::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External)

target_link_libraries(${PROJECT_NAME}
	PUBLIC mst::mst
	PUBLIC absl::inlined_vector
	PRIVATE nlohmann_json::nlohmann_json)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")