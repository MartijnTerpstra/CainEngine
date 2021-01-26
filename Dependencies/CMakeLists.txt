cmake_minimum_required (VERSION 3.8.2)

if (BUILD_TESTING)
add_subdirectory(googletest)
endif()

# mst utility library
add_subdirectory(mst)

# abseil utlity library
set(ABSL_CXX_STANDARD 17)
add_subdirectory(Abseil)

# nlohmann JSON library
add_subdirectory(nlohmann_json)

#add_subdirectory(spirv-cross)