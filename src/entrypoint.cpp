#include <iostream>
#include <spdlog/spdlog.h>
#include <bifrost/bifrost.h>
#include "constants.h"
#include "hook_typedefs.h"

std::shared_ptr<bifrost> bifrost_instance;

__attribute__((constructor))
void library_entrypoint() {
    if (std::getenv(ENV_DEBUG)) {
        spdlog::set_level(spdlog::level::debug);
    }
    spdlog::set_pattern("<bifrost> [%H:%M:%S] [%^%l%$] %v");
    
    spdlog::info("Bifrost loading...");
    bifrost_instance = std::make_shared<bifrost>();
}