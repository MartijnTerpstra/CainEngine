cmake_minimum_required (VERSION 3.25)

# C as well as CXX: wayland-scanner's generated xdg-shell bindings (see XDG_SHELL_SOURCE below)
# need to be compiled as actual C. A top-level `const` global has external linkage in C but
# internal linkage in C++ by default, and the generated xdg-shell-client-protocol.h declares
# things like `extern const struct wl_interface xdg_wm_base_interface;` expecting the former -
# compiling the matching definitions as C++ silently gives them internal linkage instead, which
# still compiles fine but fails at link time with "undefined reference" for every one of them.
project (Platform.Linux CXX C)

file(GLOB SOURCELIST "*.cpp")

find_package(X11 REQUIRED)

if(NOT X11_Xrandr_FOUND)
	message(FATAL_ERROR "Platform.Linux requires the Xrandr X11 extension (libxrandr-dev)")
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(Wayland REQUIRED IMPORTED_TARGET wayland-client)
pkg_check_modules(WaylandProtocols REQUIRED wayland-protocols)
pkg_check_modules(Xkbcommon REQUIRED IMPORTED_TARGET xkbcommon)

find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner REQUIRED)
pkg_get_variable(WAYLAND_PROTOCOLS_DATADIR wayland-protocols pkgdatadir)

# wayland-client only ships the core wayland.xml interfaces (wl_compositor, wl_output, wl_seat,
# ...); xdg-shell - the stable protocol used here for actual toplevel (application) windows - is
# distributed as XML by wayland-protocols instead, so its client bindings have to be generated at
# configure time.
set(XDG_SHELL_XML "${WAYLAND_PROTOCOLS_DATADIR}/stable/xdg-shell/xdg-shell.xml")
set(XDG_SHELL_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(XDG_SHELL_CLIENT_HEADER "${XDG_SHELL_GENERATED_DIR}/xdg-shell-client-protocol.h")
set(XDG_SHELL_SOURCE "${XDG_SHELL_GENERATED_DIR}/xdg-shell-protocol.c")

# wayland-scanner writes straight to these paths and won't create the directory itself; the
# Ninja generator happens to create OUTPUT directories for add_custom_command on its own, but
# Unix Makefiles doesn't, so this has to be explicit to work with both.
file(MAKE_DIRECTORY ${XDG_SHELL_GENERATED_DIR})

add_custom_command(
	OUTPUT ${XDG_SHELL_CLIENT_HEADER}
	COMMAND ${WAYLAND_SCANNER_EXECUTABLE} client-header ${XDG_SHELL_XML} ${XDG_SHELL_CLIENT_HEADER}
	DEPENDS ${XDG_SHELL_XML}
	VERBATIM)

add_custom_command(
	OUTPUT ${XDG_SHELL_SOURCE}
	COMMAND ${WAYLAND_SCANNER_EXECUTABLE} private-code ${XDG_SHELL_XML} ${XDG_SHELL_SOURCE}
	DEPENDS ${XDG_SHELL_XML}
	VERBATIM)

add_custom_target(Platform.Linux.XdgShellProtocol DEPENDS ${XDG_SHELL_CLIENT_HEADER} ${XDG_SHELL_SOURCE})

add_library(${PROJECT_NAME} ${SOURCELIST} ${XDG_SHELL_SOURCE})

# Ensures both generated files exist before any source in this target (in particular
# WaylandWindow.cpp, which #includes the generated header) gets compiled.
add_dependencies(${PROJECT_NAME} Platform.Linux.XdgShellProtocol)

add_library(Platform::Linux ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/External
	PUBLIC ${X11_INCLUDE_DIR}
	PUBLIC ${X11_Xrandr_INCLUDE_PATH}
	PUBLIC ${XDG_SHELL_GENERATED_DIR})

target_link_libraries(${PROJECT_NAME}
	PUBLIC Common::Common
	PUBLIC Platform::Core
	PUBLIC ${X11_LIBRARIES}
	PUBLIC ${X11_Xrandr_LIB}
	PUBLIC PkgConfig::Wayland
	PUBLIC PkgConfig::Xkbcommon)

add_precompiled_header(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Precomp.h" "${PROJECT_SOURCE_DIR}/Precomp.cpp")
