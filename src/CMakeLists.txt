set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/bifrost/bifrost.hpp")

add_library(remarkable_bifrost SHARED entrypoint.cpp ${BSWR_SOURCES}
        bifrost.cpp
        bifrost_impl.cpp
        bifrost_impl.h)

set_target_properties(remarkable_bifrost PROPERTIES OUTPUT_NAME "bifrost")

# import all Qt6 components
find_package(Qt6 COMPONENTS Core Gui REQUIRED)
target_link_libraries(remarkable_bifrost PRIVATE Qt6::Core Qt6::Gui)
target_include_directories(remarkable_bifrost PRIVATE ${Qt6Core_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})