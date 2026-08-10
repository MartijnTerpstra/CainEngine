cmake_minimum_required (VERSION 3.8.2)

if (BUILD_TESTING)
# Match the rest of the project's dynamic (/MD, /MDd) runtime library instead
# of gtest's default static one, or linking anything against gtest fails with
# LNK2038 RuntimeLibrary mismatches.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
endif()

# mst utility library
add_subdirectory(mst)

# abseil utlity library
#
# ABSL_CXX_STANDARD is Abseil's old (pre-CMAKE_CXX_STANDARD-based) compiler
# feature check; recent Abseil releases dropped it and check CMAKE_CXX_STANDARD
# directly instead, so it's set here too (scoped to just this add_subdirectory
# so it doesn't leak into mst/googletest/nlohmann_json's own builds).
set(ABSL_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
add_subdirectory(Abseil)
unset(CMAKE_CXX_STANDARD)

# nlohmann JSON library
add_subdirectory(nlohmann_json)

#add_subdirectory(spirv-cross)