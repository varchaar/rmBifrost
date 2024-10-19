set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/bifrost/bifrost.hpp")

file(GLOB BSWR_SOURCES ${PROJECT_SOURCE_DIR}/external/BSWR/*.c)
file(GLOB_RECURSE lvgl_sources ${PROJECT_SOURCE_DIR}/external/lvgl/src/*.c ${PROJECT_SOURCE_DIR}/external/lvgl/src/*.cpp)

# -Wnarrowing
add_compile_options(-Wno-narrowing)

add_library(remarkable_bifrost SHARED entrypoint.cpp
        bifrost.cpp
        bifrost_impl.cpp
        bifrost_impl.h
        gui/lvgl_app.cpp
        gui/lvgl_app.h
        ${BSWR_SOURCES}
        ${lvgl_sources}
)

target_precompile_headers(remarkable_bifrost PRIVATE "gui/lv_conf.h")

set_target_properties(remarkable_bifrost PROPERTIES OUTPUT_NAME "bifrost")

# import all Qt6 components
find_package(Qt6 COMPONENTS Core Gui REQUIRED)
target_link_libraries(remarkable_bifrost PRIVATE Qt6::Core Qt6::Gui)
target_include_directories(remarkable_bifrost PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})