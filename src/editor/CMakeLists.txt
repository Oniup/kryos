include(../../build_files/compiler.cmake)

file(GLOB_RECURSE kryos_editor_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE kryos_editor_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
add_executable(
    kryos_editor
    ${kryos_editor_HEADERS}
    ${kryos_editor_SOURCES}
)

target_compile_options( kryos_editor
    PUBLIC ${DEFAULT_COMPILE_OPTIONS}
)
target_compile_definitions(
    kryos_editor
    PUBLIC ${DEFAULT_COMPILE_DEFINITIONS}
)

set_target_properties(
    kryos_editor
    PROPERTIES VERSION ${VERSION_BUILD_INFO}
               SOVERSION ${VERSION_BUILD_INFO}
)
target_include_directories(
    kryos_editor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
           ${kryos_INCUDE_DIRS}
)
target_link_libraries(
    kryos_editor
    PUBLIC kryos
)
