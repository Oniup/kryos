include(../../build_files/compiler.cmake)

target_compile_options(
    kryos
    PUBLIC ${DEFAULT_COMPILE_OPTIONS}
)
target_compile_definitions(
    kryos
    PUBLIC ${DEFAULT_COMPILE_DEFINITIONS}
)

set_target_properties(
    kryos
    PROPERTIES VERSION ${VERSION_BUILD_INFO}
               SOVERSION ${VERSION_BUILD_INFO}
)

file(GLOB_RECURSE kryos_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE kryos_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

target_sources(
    kryos
    PUBLIC ${kryos_HEADERS}
    PRIVATE ${kryos_SOURCES}
)

target_include_directories(
    kryos
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
    kryos
)
