set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/bifrost/bifrost.hpp")

file(GLOB BSWR_SOURCES ${PROJECT_SOURCE_DIR}/external/BSWR/*.c)
file(GLOB_RECURSE lvgl_sources ${PROJECT_SOURCE_DIR}/external/lvgl/src/*.c ${PROJECT_SOURCE_DIR}/external/lvgl/src/*.cpp)

# -Wnarrowing
add_compile_options(-Wno-narrowing)
add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

add_library(rmBifrost_compositor SHARED entrypoint.cpp
        # fonts
        resources/fonts/ebgaramond_48.c

        bifrost.cpp
        bifrost_impl.cpp
        bifrost_impl.h
        gui/lvgl_renderer.cpp
        gui/lvgl_renderer.h
        ${BSWR_SOURCES}
        ${lvgl_sources}
        gui/boot_screen.cpp
        gui/boot_screen.h
        utils/shm_channel.cpp
        utils/shm_channel.h
        compositor/compositor.cpp
        compositor/compositor.h
        utils/unix_socket.cpp
        utils/unix_socket.h
        compositor/compositor_client.cpp
        compositor/compositor_client.h
        compositor/packets/packet.h
        compositor/packets/begin_session_request.h
        compositor/packets/begin_session_response.h
        compositor/packets/submit_frame_packet.h
        compositor/packets/release_frame_packet.h
        resources.h
)

add_library(rmBifrost::compositor ALIAS rmBifrost_compositor)

target_precompile_headers(rmBifrost_compositor PRIVATE "gui/lv_conf.h")

set_target_properties(rmBifrost_compositor PROPERTIES OUTPUT_NAME "bifrost")

# import all Qt6 components
find_package(Qt6 COMPONENTS Core Gui REQUIRED)
target_link_libraries(rmBifrost_compositor PRIVATE Qt6::Core Qt6::Gui)
target_include_directories(rmBifrost_compositor PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})

add_subdirectory(resources)
target_link_libraries(rmBifrost_compositor PRIVATE bifrost::resources)

add_subdirectory(client)
