cmake_minimum_required (VERSION 3.8.2)

project (CainEngine CXX)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
add_compile_definitions(_DEBUG=1)
else()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
message(FATAL_ERROR "Clang compiler optimizer settings not implemented")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
message(FATAL_ERROR "GCC compiler optimizer settings not implemented")
  # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
message(FATAL_ERROR "Intel compiler optimizer settings not implemented")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /Ot /Oi /GL /GS- /fp:fast /arch:AVX /EHs-c- /GR-")
set(CMAKE_EXE_LINKER_FLAGS "/LTCG")
add_compile_definitions(_HAS_EXCEPTIONS=0)
  # using Visual Studio C++
endif()
endif()

if (BUILD_TESTING)
enable_testing()
endif()

if(MSVC)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
endif()

# Helpers
add_subdirectory(CMake)

# Dependencies
add_subdirectory(Dependencies)

# Core libraries
add_subdirectory(Common)
add_subdirectory(EntitySystem)
add_subdirectory(Platform)
add_subdirectory(Graphics)

# Application
add_subdirectory(Content)
add_subdirectory(Engine)

if (WIN32)

# Core library implementations
add_subdirectory(Platform.Win32)
add_subdirectory(Graphics.DX11)
#add_subdirectory(Graphics.DX12)
#add_subdirectory(Graphics.Vulkan)

# Launchers
add_subdirectory(Application.Win32)
##add_subdirectory(Application.Win32Vulkan)

#editor
add_subdirectory(Editor.ShaderCompiler)
add_subdirectory(Editor)

elseif(UNIX)

# Core library implementations
add_subdirectory(Platform.Linux)
add_subdirectory(Graphics.Vulkan)

# Launchers
##add_subdirectory(Application.LinuxVulkan)

endif()